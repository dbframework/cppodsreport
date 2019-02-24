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
#ifndef ODSFILE_H
#define ODSFILE_H

#include "cziparchive.h"

#include <memory>
#include <vector>

#include "xmldom.h"
#include "odstable.h"
#include "datasourceabstract.h"
#include "cppodsreportcore.h"

namespace cppodsreport {

class ODSFile : public ODFPackage
{
private:
    ODSFile(ODSFile&){};
    cppzip::CZipArchive m_zip;
    bool m_isOpen;
    int m_error;
    bool m_zipError;

    typedef std::unique_ptr<char[]> Buffer;

    bool readFile(const char* fileName, DOMDocumentWrapper &content);
    bool writeFile(const char* fileName, DOMDocumentWrapper &content, Buffer& buf);

    DOMDocumentWrapper m_content;
        
    ODSTable::ODSTableMap m_tables;
    ODSTable::ODSTableVector m_tablesVector;

    void parseTables(const DomNodeList& tables);
    void parseContent();

    std::unique_ptr<char[]> m_contentBuf;
    bool doOpen();
    void setError(int error = 0, bool zipError = false);
    bool doSave(bool createRes);
    void prepareSave();

    bool createMIMETypeFile();
public:
    ODSFile();
    ~ODSFile();
    bool open(const char* fileName);
    bool save(const char* fileName);
#ifdef CPPODSREPORT_WIN
    bool opena(const char* fileName);
    bool open(const wchar_t* fileName);
    bool savea(const char* fileName);
    bool save(const wchar_t* fileName);
#endif
    bool process(DataSource* ds);
    bool save();
    bool close();
    struct ErrorStruct {
        int internalError;
        int internalError2;
        int zipError;
        int zlibError;
        int sysError;
    };
    void getLastError(ErrorStruct& error) const;

    ODSSize sheetCount();
    ODSSheet& sheet(ODSSize sheetIndex);

    DOMDocumentWrapper& content();

    
};

};
#endif // ODSFILE_H
