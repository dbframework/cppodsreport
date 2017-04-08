/*
Copyright (c) 2017 Sidorov Dmitry

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
#include "odscell.h"
#include "odsconst.h"

using namespace cppodsreport;
using namespace std;

void ODSCell::init()
{
    m_repeatCount = 1;
    m_valueType = VT_STRING;
}

ODSCell::ODSCell()
{
    init();
}

ODSCell::ODSCell(DomDocument& doc)
{    
    init();
    m_cellNode = doc.createElementNS(ODS_NS_TABLE, ODS_ELEMENT_CELL);
}

ODSCell::ODSCell(const DomElement& cellNode)
{    
    init();
    m_cellNode = cellNode;
    DomString s = m_cellNode.getAttributeNS(ODS_NS_TABLE, ODS_ATTR_COL_REP);
    wstring ws = DOMDocumentWrapper::DomStringToWstring(s);
    if (!ws.empty()) {
        size_t idx;
        m_repeatCount = stoul(ws, &idx);
        if (idx < ws.size())
            m_repeatCount = 1;
    }
    s = m_cellNode.getAttributeNS(ODS_NS_TABLE, ODS_ATTR_FORMULA);
    ws = DOMDocumentWrapper::DomStringToWstring(s);
    if (!ws.empty())
        m_formula.reset(new ODSFormula(ws));

    DomNode textNode = getCellTextNode();
    if (textNode.nodeType() == NODE_TYPE_TEXT) {
        m_text = DOMDocumentWrapper::DomStringToWstring(textNode.nodeValue());
    }
}

void ODSCell::assign(const ODSCell& cell)
{    
    m_cellNode = DOMDocumentWrapper::toElement(cell.m_cellNode.cloneNode());
    m_repeatCount = cell.repeatCount();
    m_text = cell.m_text;
    if (cell.m_formula.get() != nullptr) {
        m_formula.reset(new ODSFormula(*cell.m_formula));
        *m_formula = *cell.m_formula;
    }
    m_valueType = cell.m_valueType;
}

ODSCell::ODSCell(const ODSCell& cell)
{
    assign(cell);
}

ODSCell& ODSCell::operator=(const ODSCell& cell)
{
   assign(cell);
   return *this;
}

DomElement ODSCell::cellNode()
{
    return m_cellNode;
}

size_t ODSCell::repeatCount() const
{
    return m_repeatCount;
}

void ODSCell::setRepeatCount(size_t value)
{
    m_repeatCount = value;    
}

DomNode ODSCell::getCellTextNode()
{
    DomNode result;

    DomNodeList list = m_cellNode.elementsByTagNameNS(ODS_NS_TEXT, ODS_ELEMENT_TEXTP);
    if (list.length() > 0) {
        DomNode textp = list.item(0);
        DomNodeList items = textp.childNodes();
        for (int i = 0; i < items.length(); ++i) {
            if (items.item(i).nodeType() == NODE_TYPE_TEXT) {
                result = items.item(i);
                break;
            }
        }

    }

    return result;
}

wstring ODSCell::textp()
{    
    return m_text;
}

void ODSCell::setTextp(const wstring &text)
{    
    m_text = text;
    checkText();
}

void ODSCell::save()
{
    saveText();
    if (m_repeatCount > 1) {
        m_cellNode.setAttributeNS(ODS_NS_TABLE, ODS_ATTR_COL_REP, DOMDocumentWrapper().WstringToDomString(to_wstring(m_repeatCount)));
    }
    else {
        m_cellNode.removeAttributeNS(ODS_NS_TABLE, ODS_ATTR_COL_REP);
    }

    if (m_formula.get() != nullptr) {
        m_formula->save();
        m_cellNode.setAttributeNS(ODS_NS_TABLE, ODS_ATTR_FORMULA, DOMDocumentWrapper().WstringToDomString(m_formula->formula()));
    }
}

ODSFormulaSPtr ODSCell::formula() const
{
    return m_formula;
}

void ODSCell::saveText()
{
    DomNode textNode = getCellTextNode();
    if (textNode.nodeType() == NODE_TYPE_TEXT) {
        textNode.setNodeValue(DOMDocumentWrapper::WstringToDomString(m_text));
    }

    if (VT_FLOAT == m_valueType) {
        m_cellNode.setAttributeNS(ODS_NS_OFFICE, ODS_ATTR_VALUE_TYPE, "float");
        m_cellNode.setAttributeNS(ODS_NS_OFFICE, ODS_ATTR_VALUE, DOMDocumentWrapper::WstringToDomString(m_text));
    }
}

void ODSCell::checkText()
{
    try {
        stod(m_text);
        //it's a number
        m_valueType = VT_FLOAT;
    }
    catch(std::exception& ){

    }
}
