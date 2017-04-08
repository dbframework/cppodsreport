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
#ifndef ODSCELLADDRESS_H
#define ODSCELLADDRESS_H

#include <string>
#include <bitset>

namespace cppodsreport {

class ODSCellAddress {
public:
    typedef unsigned int CellIndex;
private:
    enum FlagIndex {
        ColumnAbsAddress = 0,
        RowAbsAddress = 1,
        ColumnAddress = 2,
        RowAddress = 3
    };
    std::wstring m_Sheet;
    CellIndex m_Column;
    CellIndex m_Row;
    std::bitset<4> m_Flags;
    CellIndex decodeColumn(std::wstring col) const;
    std::wstring encodeColumn(CellIndex col) const;    
    std::wstring quote(const std::wstring s) const;
public:
    ODSCellAddress();
    bool setValue(const std::wstring& str);
    bool operator==(const ODSCellAddress& rhs) const;
    bool operator!=(const ODSCellAddress& rhs) const;
    bool valid() const;
    CellIndex column() const;
    CellIndex row() const;
    std::wstring sheet() const;
    void setColumn(CellIndex col);
    void setRow(CellIndex row);
    void setSheet(const std::wstring& sheet);
    std::wstring toWString() const;
    bool hasRow() const;
    bool hasColumn() const;
    bool absoluteRow() const;
    bool absoluteColumn() const;
};

}
#endif // ODSCELLADDRESS_H
