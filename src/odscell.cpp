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
#include "odscell.h"
#include "odsconst.h"
#include <string> 
#include <sstream>
#include <iomanip>

using namespace cppodsreport;
using namespace std;

const char* ODSCell::ValueTypes[] = {ODS_VALUE_TYPE_BOOL, ODS_VALUE_TYPE_CUR, ODS_VALUE_TYPE_DATE, ODS_VALUE_TYPE_FLT, ODS_VALUE_TYPE_PRC, 
    ODS_VALUE_TYPE_STR, ODS_VALUE_TYPE_TIME, ODS_VALUE_TYPE_VOID, nullptr};
const char* ODSCell::ValueAttributes[] = { ODS_ATTR_VALUE_BOOL, ODS_ATTR_VALUE, ODS_ATTR_VALUE_DATE, ODS_ATTR_VALUE, ODS_ATTR_VALUE, ODS_ATTR_VALUE_STR, ODS_ATTR_VALUE_TIME, "", nullptr };

void ODSCell::init()
{
    m_repeatCount = 1;
    m_valueType = VT_STRING;
    m_text.clear();
    m_formula.reset();
    m_currency.clear();
}

ODSCell::ODSCell()
{
    init();
}

ODSCell::ODSCell(DomDocument& doc)
{    
    init();
    m_cellNode = doc.createElementNS(ODS_NS_TABLE, DOMDocumentWrapper::qualifiedName(ODS_NSP_TABLE, ODS_ELEMENT_CELL));
}

void ODSCell::readValueFromAttribute(DomElement& cellNode)
{
    m_text = DOMDocumentWrapper::DomStringToWstring(cellNode.getAttributeNS(ODS_NS_OFFICE, ValueAttributes[m_valueType]));
    if (VT_CURRENCY == m_valueType)
        m_currency = DOMDocumentWrapper::DomStringToWstring(cellNode.getAttributeNS(ODS_NS_OFFICE, ODS_ATTR_VALUE_CUR));
}

void ODSCell::readValueFromElement(DomElement& cellNode)
{
    DomNode textNode = getCellTextNode();
    if (textNode.nodeType() == NODE_TYPE_TEXT) {
        m_text = DOMDocumentWrapper::DomStringToWstring(textNode.nodeValue());
    }
}

void ODSCell::getCellValue(DomElement& cellNode)
{
    DomString s = cellNode.getAttributeNS(ODS_NS_OFFICE, ODS_ATTR_VALUE_TYPE);

    for (int i = 0; ValueTypes[i] != nullptr; i++) {
        if (DomString(ValueTypes[i]) == s) {
            m_valueType = static_cast<ValueType>(i);            
        }
    }

    if (DomString("") == s) {
        m_valueType = VT_STRING;
    }
    
    if (VT_VOID != m_valueType) {
        readValueFromAttribute(cellNode);
        if (m_text.empty())
            readValueFromElement(cellNode);
    }
}

ODSCell::ODSCell(const DomElement& cellNode)
{    
    init();
    m_cellNode = cellNode;
    DomString s = m_cellNode.getAttributeNS(ODS_NS_TABLE, ODS_ATTR_COL_REP);
    wstring ws = DOMDocumentWrapper::DomStringToWstring(s);
    if (!ws.empty()) {
        size_t idx;
        m_repeatCount = std::stoul(ws, &idx);
        if (idx < ws.size())
            m_repeatCount = 1;
    }
    s = m_cellNode.getAttributeNS(ODS_NS_TABLE, ODS_ATTR_FORMULA);
    ws = DOMDocumentWrapper::DomStringToWstring(s);
    if (!ws.empty())
        m_formula.reset(new ODSFormula(ws));
        
    getCellValue(m_cellNode);
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
    m_currency = cell.m_currency;
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
        m_cellNode.setAttributeNS(ODS_NS_TABLE, DOMDocumentWrapper::qualifiedName(ODS_NSP_TABLE,ODS_ATTR_COL_REP), DOMDocumentWrapper().WstringToDomString(to_wstring(m_repeatCount)));
    }
    else {
        m_cellNode.removeAttributeNS(ODS_NS_TABLE, ODS_ATTR_COL_REP);
    }

    if (m_formula.get() != nullptr) {
        m_formula->save();
        m_cellNode.setAttributeNS(ODS_NS_TABLE, DOMDocumentWrapper::qualifiedName(ODS_NSP_TABLE, ODS_ATTR_FORMULA), DOMDocumentWrapper().WstringToDomString(m_formula->formula()));
    }
}

ODSFormulaSPtr ODSCell::formula() const
{
    return m_formula;
}

void ODSCell::saveValueToAttr()
{
    if (!m_text.empty()) {
        m_cellNode.setAttributeNS(ODS_NS_OFFICE, DOMDocumentWrapper::qualifiedName(ODS_NSP_OFFICE, ODS_ATTR_VALUE_TYPE), DomString(ValueTypes[m_valueType]));
    }
    else {
        m_cellNode.removeAttributeNS(ODS_NSP_OFFICE, ODS_ATTR_VALUE_TYPE);
    }

    m_cellNode.removeAttributeNS(ODS_NS_OFFICE, ODS_ATTR_VALUE);
    m_cellNode.removeAttributeNS(ODS_NS_OFFICE, ODS_ATTR_VALUE_BOOL);
    m_cellNode.removeAttributeNS(ODS_NS_OFFICE, ODS_ATTR_VALUE_CUR);
    m_cellNode.removeAttributeNS(ODS_NS_OFFICE, ODS_ATTR_VALUE_DATE);
    m_cellNode.removeAttributeNS(ODS_NS_OFFICE, ODS_ATTR_VALUE_STR);
    m_cellNode.removeAttributeNS(ODS_NS_OFFICE, ODS_ATTR_VALUE_TIME);

    if ((m_valueType != VT_STRING) && (m_valueType != VT_VOID) && !m_text.empty()) {
        m_cellNode.setAttributeNS(ODS_NS_OFFICE, DOMDocumentWrapper::qualifiedName(ODS_NSP_OFFICE, ValueAttributes[m_valueType]), DOMDocumentWrapper::WstringToDomString(m_text));
    }

    if (VT_CURRENCY == m_valueType) {
        m_cellNode.setAttributeNS(ODS_NS_OFFICE, DOMDocumentWrapper::qualifiedName(ODS_NSP_OFFICE, ODS_ATTR_VALUE_CUR), DOMDocumentWrapper::WstringToDomString(m_currency));
    }
}

void ODSCell::saveText()
{
    DomNode textNode = getCellTextNode();

    if (textNode.isNull() && !m_text.empty()) {
        textNode = m_cellNode.appendChild(m_cellNode.ownerDocument().createElementNS(ODS_NS_TEXT, DOMDocumentWrapper::qualifiedName(ODS_NSP_TEXT, ODS_ELEMENT_TEXTP)));
        textNode = textNode.appendChild(m_cellNode.ownerDocument().createTextNode(DomString()));
    }

    if (textNode.nodeType() == NODE_TYPE_TEXT) {
        if (VT_CURRENCY == m_valueType) {
            textNode.setNodeValue(DOMDocumentWrapper::WstringToDomString(m_text + m_currency));
        }
        else {
            textNode.setNodeValue(DOMDocumentWrapper::WstringToDomString(m_text));
        }
    }
    
    saveValueToAttr();
}

void ODSCell::checkText()
{
    try {
        size_t pos;
        stod(m_text, &pos);
        //whole m_text is a number, not only m_text start
        if (pos >= m_text.size())
            m_valueType = VT_FLOAT;
    }
    catch(std::exception& ){

    }
}

const wchar_t* ODSCell::text() const
{
    return m_text.c_str();
}

void ODSCell::setText(const wchar_t* text)
{
    m_text = text;
}

ODSCell::ValueType ODSCell::type() const
{
    return m_valueType;
}

void ODSCell::setBool(bool value)
{
    m_valueType = VT_BOOL;
    if (value) {
        m_text = L"true";
    }
    else {
        m_text = L"false";
    }
}

void ODSCell::setFloat(double value)
{
    m_valueType = VT_FLOAT;
    m_text = to_wstring(value);
}

void ODSCell::setPercentage(double value)
{
    m_valueType = VT_PERCENTAGE;
    m_text = to_wstring(value);
}

void ODSCell::setCurrency(double value, const wchar_t* currency)
{
    m_valueType = VT_CURRENCY;
    m_text = to_wstring(value);
    m_currency = wstring(currency);
}

void ODSCell::setDate(int year, unsigned char month, unsigned char day)
{
    m_valueType = VT_DATE;
    wostringstream s;
    s << setfill(L'0') << setw(4) << year << L"-" << setw(2) << month << L"-" << setw(2) << day;
    m_text = s.str();
}

void ODSCell::setDate(int year, unsigned char month, unsigned char day,
    unsigned char hour, unsigned char minute, float second)
{
    setDate(year, month, day);
    wostringstream s;
    s << L'T' << setfill(L'0') << setw(2) << hour << L":" << setw(2) << minute << L":" << fixed << second;
    m_text += s.str();
}

void ODSCell::setTime(unsigned char hour, unsigned char minute, float second)
{
    m_valueType = VT_TIME;
    wostringstream s;
    s << L"PT" << hour << L"H" << minute << L"M" << fixed << second << L"S";
    m_text = s.str();
}