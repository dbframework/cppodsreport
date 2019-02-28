/*
Copyright (c) 2017-2019 Sidorov Dmitry

Permission is hereby granted, free of charge, to any person obtaining a copy of this software
and associated documentation files (the "Software"), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial 
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN 
NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.  
*/
#include "odsfile.h"
#include "odsconst.h"
#include <memory>
#include "odserror.h"

using namespace cppodsreport;
using namespace cppzip;
using namespace std;

const char* MIMETypeSpreadsheet = "application/vnd.oasis.opendocument.spreadsheet";

ODSFile::ODSFile()
{
    m_isOpen = false;
    m_error = 0;
    m_zipError = false;
}

ODSFile::~ODSFile()
{    
    close();
}

bool ODSFile::readFile(const char* fileName, DOMDocumentWrapper& content)
{
    bool result = false;
    ZipFileStat stat;
    unique_ptr<ZipArchiveFile> f;
    CZipArchive::ZipInt index = m_zip.findFileInZip(fileName);
    ZipFileStat::ZipInt64 size = -1;
    unique_ptr<char[]> buf;

    if (-1 != index) {
        result = m_zip.fileStat(index, stat);
        size = stat.uncompressedSize();
    }

    if (size != -1) {
        buf.reset(new char[size]);
        f.reset(m_zip.openFileInZip(index));
    }

    if (f.get() != nullptr) {
        result = (size == f->read(buf.get(), stat.uncompressedSize()));
    }

    if(result) {
        result = content.setContent(buf.get(), size);
    }

    return result;
}

bool ODSFile::doOpen()
{
    if (m_isOpen) {
       m_isOpen = readFile(ODSFILE_CONTENT_FILE_NAME, m_content);
       if (!m_isOpen)
           setError(ODSERR_CONTENT_READ, true);
    }
    else {
        setError(ODSERR_ARCH_OPEN, true);
    }

    if (m_isOpen) {
        parseContent();
    }

    return m_isOpen;
}

bool ODSFile::open(const char* fileName)
{
    setError();
    if (m_isOpen)
        close();

    m_isOpen = m_zip.open(fileName);

    return doOpen();
}

#ifdef CPPODSREPORT_WIN
bool ODSFile::opena(const char* fileName)
{
    setError();
    if (m_isOpen)
        close();

    m_isOpen = m_zip.opena(fileName);

    return doOpen();
}

bool ODSFile::open(const wchar_t* fileName)
{
    setError();
    if (m_isOpen)
        close();

    m_isOpen = m_zip.open(fileName);

    return doOpen();
}
#endif

bool ODSFile::close()
{    
    bool result = m_zip.close();
    if (!result) {
        setError(ODSERR_ARCH_SAVE, true);        
    }
    m_content.clearContent();
    m_tables.clear();
    m_tablesVector.clear();
    m_isOpen = false;
    return result;
}

bool ODSFile::commonSave()
{
    bool result = false;

    DomNodeList doccont = m_content.document().elementsByTagNameNS(ODS_NS_OFFICE, ODS_ELEMENT_DOC_CONTENT);
    if (doccont.length()) {
        DomElement elem = m_content.toElement(doccont.item(0));
        DomString xmlns("xmlns:");
        elem.setAttribute(xmlns + ODS_NSP_STYLE, ODS_NS_STYLE);
        elem.setAttribute(xmlns + ODS_NSP_TEXT, ODS_NS_TEXT);
        elem.setAttribute(xmlns + ODS_NSP_TABLE, ODS_NS_TABLE);
        elem.setAttribute(xmlns + ODS_NSP_SCRIPT, ODS_NS_SCRIPT);
        elem.setAttribute(xmlns + ODS_NSP_OF, ODS_NS_OF);
        elem.setAttribute(xmlns + ODS_NSP_STYLE_XSL_COMP, ODS_NS_STYLE_XSL_COMP);
        elem.setAttribute(xmlns + ODS_NSP_DRAW, ODS_NS_DRAW);
        elem.setAttribute(xmlns + ODS_NSP_XLINK, ODS_NS_XLINK);
        elem.setAttribute(xmlns + ODS_NSP_DC, ODS_NS_DC);
        elem.setAttribute(xmlns + ODS_NSP_META, ODS_NS_META);
        elem.setAttribute(xmlns + ODS_NSP_NUMBER, ODS_NS_NUMBER);
        elem.setAttribute(xmlns + ODS_NSP_PRESENTATION, ODS_NS_PRESENTATION);
        elem.setAttribute(xmlns + ODS_NSP_SVG, ODS_NS_SVG);
        elem.setAttribute(xmlns + ODS_NSP_CHART, ODS_NS_CHART);
        elem.setAttribute(xmlns + ODS_NSP_DR3D, ODS_NS_DR3D);
        elem.setAttribute(xmlns + ODS_NSP_MATH, ODS_NS_MATH);
        elem.setAttribute(xmlns + ODS_NSP_FORM, ODS_NS_FORM);
    }

    for (ODSTable &t : m_tablesVector)
        t.save();

    result = writeFile(ODSFILE_CONTENT_FILE_NAME, m_content, m_contentBuf);
    if (!result) {
        setError(ODSERR_CONTENT_WRITE, m_zipError);
    }

    return result;
}

bool ODSFile::save()
{
    setError();
    bool result = false;

    if(!m_isOpen) {
        setError(ODSERR_NOT_OPENED, false);
        return result;
    }         
            
    return commonSave();
}

bool ODSFile::writeFile(const char* fileName, DOMDocumentWrapper &content, Buffer &buf)
{
    bool result = false;

    int size = content.contentSize();
    buf.reset(new char[size]);
    if (size == content.getContent(buf.get(), size)) {
        result = (m_zip.addFile(buf.get(), size, fileName, true) >= 0);
        if (!result)
            setError(0, true);
    }

    return result;
}

void ODSFile::parseContent()
{
    DomNodeList tables = m_content.document().elementsByTagNameNS(ODS_NS_TABLE, ODS_ELEMENT_TABLE);
    parseTables(tables);
}

void ODSFile::parseTables(const DomNodeList& tables)
{
    m_tables.clear();
    m_tables.reserve(tables.length());
    m_tablesVector.clear();
    m_tablesVector.reserve(tables.length());
    for (int i = 0; i < tables.length(); ++i) {
        m_tablesVector.push_back(ODSTable(this, m_content.toElement(tables.item(i))));
        m_tables.insert(make_pair(m_tablesVector.back().name(), m_tablesVector.size() - 1));         
    }
    for (ODSTable &t : m_tablesVector)
        t.parseTable(m_tables, m_tablesVector);    
}

bool ODSFile::process(DataSource* ds)
{
    if (nullptr == ds) {
        setError(ODSERR_DS_NULL, false);
        return false;
    }
    for (ODSTable &t : m_tablesVector)
        t.assignVars(ds, m_tables, m_tablesVector);
    
    return true;
}

void ODSFile::setError(int error, bool zipError)
{
    m_error = error;
    m_zipError = zipError;
}

void ODSFile::getLastError(ErrorStruct& error) const
{
    error.internalError = m_error;
    if (m_zipError) {
        error.internalError2 = m_zip.internalError();
        error.sysError = m_zip.sysError();
        error.zipError = m_zip.zipError();
        error.zlibError = m_zip.zlibError();
    }
    else {
        error.internalError2 = error.sysError = error.zipError = error.zlibError = 0;
    }
}

ODSSize ODSFile::sheetCount()
{
    return m_tablesVector.size();
}

ODSSheet& ODSFile::sheet(ODSSize sheetIndex)
{
    return m_tablesVector[sheetIndex];
}

DOMDocumentWrapper& ODSFile::content()
{
    return m_content;
}

void ODSFile::prepareSave()
{
    setError();
    if (m_isOpen) {
        m_zip.close();
        m_isOpen = false;
    }
}

bool ODSFile::save(const char* fileName)
{    
    bool result = false;

    prepareSave();
    result = m_zip.open(fileName, CZipArchive::opCreate | CZipArchive::opFailIfExists);

    return doSave(result);
}

#ifdef CPPODSREPORT_WIN
bool ODSFile::savea(const char* fileName)
{
    bool result = false;

    prepareSave();
    result = m_zip.opena(fileName, CZipArchive::opCreate | CZipArchive::opFailIfExists);

    return doSave(result);
}


bool ODSFile::save(const wchar_t* fileName)
{
    bool result = false;

    prepareSave();
    result = m_zip.open(fileName, CZipArchive::opCreate | CZipArchive::opFailIfExists);

    return doSave(result);
}
#endif

bool ODSFile::doSave(bool createRes)
{
    bool result = createRes;

    m_isOpen = createRes;

    if (result)
        result = createMIMETypeFile();
    
    if (result && !m_content.contentSize())
        result = createContent();

    if (result)
        result = createRDFMetadata();

    if (!result) {        
        m_zip.unchange_all();
        close();
        setError(ODSERR_ARCH_CREATE, true);
    }
    else {
        commonSave();
    }

    return result;
}

bool ODSFile::createMIMETypeFile()
{
    bool result = false;
    CZipArchive::ZipInt index = m_zip.addFile(MIMETypeSpreadsheet, strlen(MIMETypeSpreadsheet), ODSFILE_MIMETYPE_FILE_NAME, true);
    if (index >= 0) {
        result = m_zip.setCompression(index, CZipArchive::caStore);
    }

    return result;
}

bool ODSFile::createContent()
{    
    DomElement el = DOMDocumentWrapper::toElement(m_content.document().appendChild(m_content.document().createElement(ODS_ELEMENT_XML)));
    el.setAttribute(ODS_ATTR_VERSION, "1.0");

    el = DOMDocumentWrapper::toElement(m_content.document().appendChild(m_content.document().createElementNS(ODS_NS_OFFICE, 
        DOMDocumentWrapper::qualifiedName(ODS_NSP_OFFICE, ODS_ELEMENT_DOC_CONTENT))));
    el.setAttributeNS(ODS_NS_OFFICE, DOMDocumentWrapper::qualifiedName(ODS_NSP_OFFICE, ODS_ATTR_VERSION), "1.2");

    el = DOMDocumentWrapper::toElement(el.appendChild(m_content.document().createElementNS(ODS_NS_OFFICE,
        DOMDocumentWrapper::qualifiedName(ODS_NSP_OFFICE, ODS_ELEMENT_BODY))));

    el = DOMDocumentWrapper::toElement(el.appendChild(m_content.document().createElementNS(ODS_NS_RDF,
        DOMDocumentWrapper::qualifiedName(ODS_NSP_RDF, ODS_ELEMENT_SPREADSHEET))));

    return true;
}

bool ODSFile::createRDFMetadata()
{
    DOMDocumentWrapper rdf;

    DomElement el = rdf.toElement(rdf.document().appendChild(rdf.document().createElement(ODS_ELEMENT_XML)));
    el.setAttribute(ODS_ATTR_VERSION, "1.0");

    DomElement root = DOMDocumentWrapper::toElement(rdf.document().appendChild(rdf.document().createElementNS(ODS_NS_OFFICE,
        DOMDocumentWrapper::qualifiedName(ODS_NSP_OFFICE, ODS_ELEMENT_RDF))));

    el = rdf.toElement(root.appendChild(rdf.document().createElementNS(ODS_NS_RDF, rdf.qualifiedName(ODS_NSP_RDF, ODS_ELEMENT_RDF_DESC))));
    el.setAttributeNS(ODS_NS_RDF, rdf.qualifiedName(ODS_NSP_RDF, ODS_ATTR_RDF_ABOUT), ODSFILE_CONTENT_FILE_NAME);
    el = rdf.toElement(el.appendChild(rdf.document().createElementNS(ODS_NS_RDF, rdf.qualifiedName(ODS_NSP_RDF, ODS_ELEMENT_RDF_TYPE))));
    el.setAttributeNS(ODS_NS_RDF, rdf.qualifiedName(ODS_NSP_RDF, ODS_ATTR_RDF_RESOURCE), ODS_RDF_RESOURCE_CONTENT);

    el = rdf.toElement(root.appendChild(rdf.document().createElementNS(ODS_NS_RDF, rdf.qualifiedName(ODS_NSP_RDF, ODS_ELEMENT_RDF_DESC))));
    el.setAttributeNS(ODS_NS_RDF, rdf.qualifiedName(ODS_NSP_RDF, ODS_ATTR_RDF_ABOUT), "");
    el = rdf.toElement(el.appendChild(rdf.document().createElementNS(ODS_NS_RDF_NS0, rdf.qualifiedName(ODS_NSP_RDF_NS0, ODS_ELEMENT_RDF_HASPART))));
    el.setAttributeNS(ODS_NS_RDF, rdf.qualifiedName(ODS_NSP_RDF, ODS_ATTR_RDF_RESOURCE), ODSFILE_CONTENT_FILE_NAME);

    el = rdf.toElement(root.appendChild(rdf.document().createElementNS(ODS_NS_RDF, rdf.qualifiedName(ODS_NSP_RDF, ODS_ELEMENT_RDF_DESC))));
    el.setAttributeNS(ODS_NS_RDF, rdf.qualifiedName(ODS_NSP_RDF, ODS_ATTR_RDF_ABOUT), "");
    el = rdf.toElement(el.appendChild(rdf.document().createElementNS(ODS_NS_RDF, rdf.qualifiedName(ODS_NSP_RDF, ODS_ELEMENT_RDF_TYPE))));
    el.setAttributeNS(ODS_NS_RDF, rdf.qualifiedName(ODS_NSP_RDF, ODS_ATTR_RDF_RESOURCE), ODS_RDF_RESOURCE_DOCUMENT);

    return writeFile(ODSFILE_RDF_FILE_NAME, rdf, m_rdfBuf);
}