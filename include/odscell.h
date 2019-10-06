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
#ifndef ODSCELL_H
#define ODSCELL_H

#include "xmldom.h"
#include "odsformula.h"
#include "cppodsreportcore.h"

namespace cppodsreport {

class ODSCell : public ODSSheetCell
{
private:
    DomElement m_cellNode;
    size_t m_repeatCount;
    ODSFormulaSPtr m_formula;
    std::wstring m_text;
    DomNode getCellTextNode();    
    ValueType m_valueType;
    static const char* ValueTypes[];
    static const char* ValueAttributes[];
    std::wstring m_currency;
    
    void assign(const ODSCell& cell);
    void saveText();
    void checkText();
    void init();  
    void getCellValue(DomElement& cellNode);
    void readValueFromAttribute(DomElement& cellNode);
    void readValueFromElement(DomElement& cellNode);
    void saveValueToAttr();
public:
    ODSCell();
    ODSCell(const ODSCell& cell);
    ODSCell& operator=(const ODSCell&);
    ODSCell(DomDocument& doc);
    ODSCell(const DomElement& cellNode);
    DomElement cellNode();
    size_t repeatCount() const;
    void setRepeatCount(size_t value);
    std::wstring textp();
    void setTextp(const std::wstring& text);
    void save();
    ODSFormulaSPtr formula() const;

    const wchar_t* text() const;
    void setText(const wchar_t* text);

    ValueType type() const;
    void setBool(bool value);
    void setFloat(double value);
    void setPercentage(double value);
    void setCurrency(double value, const wchar_t* currency);
    void setDate(int year, unsigned char month, unsigned char day);
    void setDate(int year, unsigned char month, unsigned char day, 
        unsigned char hour, unsigned char minute, float second);
    void setTime(unsigned char hour, unsigned char minute, float second);
};

}

#endif // ODSCELL_H
