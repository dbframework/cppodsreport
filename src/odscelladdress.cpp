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
#include "odscelladdress.h"

using namespace cppodsreport;
using namespace std;

ODSCellAddress::ODSCellAddress()
{
    m_Column = m_Row = 0;
}

bool ODSCellAddress::setValue(const std::wstring& str)
{
    bool result = false;

    wstring::size_type addrStart = str.rfind(L'.');
    if (addrStart != wstring::npos) {
        setSheet(str.substr(0, addrStart));

        enum parseState {
            Column$,
            Column,
            Row
        } state = Column$;
        result = true;
        wstring buf, RowBuf, ColBuf;

        for (wstring::size_type i = addrStart + 1; i < str.size(); ++i) {
            if (Column$ == state) {
                if ((str[i] == L'$') ||  ((str[i] >= L'a') && (str[i] <= L'z')) ||
                    ((str[i] >= L'A') && (str[i] <= L'Z')) ) {
                    state = Column;
                }
                else if ((str[i] >= L'0') && (str[i] <= L'9')) {
                    state = Row;
                    ColBuf = buf;
                    buf.clear();
                }
                else {
                    result = false;
                    break;
                }
            }
            else if (Column == state) {
                if ( ((str[i] >= L'a') && (str[i] <= L'z')) ||
                    ((str[i] >= L'A') && (str[i] <= L'Z')) ) {
                    //keep same state
                }
                else if ((str[i] == L'$') || ((str[i] >= L'0') && (str[i] <= L'9'))) {
                    state = Row;
                    ColBuf = buf;
                    buf.clear();
                }
                else {
                    result = false;
                    break;
                }
            }
            else if (Row == state) {
                if ((str[i] >= L'0') && (str[i] <= L'9')) {
                    //keep same state
                }
                else {
                    result = false;
                    break;
                }
            }

            buf += str[i];
        }//for

        if (result) {
            if (Row == state) {
                RowBuf = buf;
            }
            else {
                ColBuf = buf;
            }

            result = (!ColBuf.empty() || !RowBuf.empty());
        }

        if(result) {
            if (!ColBuf.empty()) {
                m_Flags[ColumnAddress] = true;
                if (ColBuf[0] == L'$') {
                    m_Flags[ColumnAbsAddress] = true;
                    ColBuf.erase(0,1);
                }
                else {
                    m_Flags[ColumnAbsAddress] = false;
                }
                m_Column = decodeColumn(ColBuf);
            }
            else {
                m_Flags[ColumnAddress] = false;
            }

            if (!RowBuf.empty()) {
                m_Flags[RowAddress] = true;
                size_t idx;
                if (RowBuf[0] == L'$') {
                    m_Flags[ODSCellAddress::RowAbsAddress] =  true;
                    m_Row = stoul(RowBuf.substr(1), &idx) - 1;
                    result = (idx >= RowBuf.size() - 1);
                }
                else {
                    m_Flags[ODSCellAddress::RowAbsAddress] =  false;
                    m_Row = stoul(RowBuf, &idx) - 1;
                    result = (idx >= RowBuf.size());
                }
            }
            else {
                m_Flags[RowAddress] = false;
            }
        }
    }

    if (!result) {
        m_Flags[ColumnAddress] = false;
        m_Flags[RowAddress] = false;
    }
    return result;
}

ODSCellAddress::CellIndex ODSCellAddress::decodeColumn(std::wstring col) const
{
    ODSCellAddress::CellIndex result = 0;
    ODSCellAddress::CellIndex power = 1;
    const int englishLetters = 26;

    for (wstring::reverse_iterator i = col.rbegin(); i != col.rend(); ++i, power *= englishLetters) {
        ODSCellAddress::CellIndex digit;
        if ((*i >= L'a') && (*i <= L'z')) {
            digit = *i - L'a';
        }
        else {
            digit = *i - L'A';
        }
        result += digit * power;
    }
    return result;
}

bool ODSCellAddress::valid() const
{
    return m_Flags[RowAddress] || m_Flags[ColumnAddress];
}

ODSCellAddress::CellIndex ODSCellAddress::column() const
{
    return m_Column;
}

ODSCellAddress::CellIndex ODSCellAddress::row() const
{
    return m_Row;
}

std::wstring ODSCellAddress::sheet() const
{
    return m_Sheet;
}

void ODSCellAddress::setColumn(CellIndex col)
{
    m_Flags[ColumnAddress] = true;
    m_Column = col;
}

void ODSCellAddress::setRow(CellIndex row)
{
    m_Flags[RowAddress] = true;
    m_Row = row;
}

void ODSCellAddress::setSheet(const std::wstring& sheet)
{
    if (sheet.size() > 1) {
        if ((sheet.front() == L'\'') && (sheet.back() == L'\'')) {
            m_Sheet = sheet.substr(1, sheet.size() - 2);
            wstring::size_type n;
            for (;(n = m_Sheet.find(L"\'\'")) != wstring::npos;) {
                m_Sheet.replace(n, 2, L"\'");
            }
         }
    }
    else
        m_Sheet = sheet;
}

std::wstring ODSCellAddress::toWString() const
{
    wstring result;
    if (!valid())
        return result;

    result = quote(m_Sheet) + L".";
    if (m_Flags[ColumnAddress]) {
        if (m_Flags[ColumnAbsAddress])
            result += L"$";
        result += encodeColumn(m_Column);
    }
    if (m_Flags[RowAddress]) {
        if(m_Flags[RowAbsAddress])
            result += L"$";
        result += to_wstring(m_Row + 1);
    }
    return result;
}

std::wstring ODSCellAddress::encodeColumn(CellIndex col) const
{
    wstring rev;
    wstring result;
    const int englishLetters = 26;

    do  {
        CellIndex digit = col % englishLetters;
        rev += 'A' + digit;
        col /= englishLetters;
    } while (col > 0);

    result.assign(rev.rbegin(), rev.rend());
    return result;
}

bool ODSCellAddress::operator==(const ODSCellAddress& rhs) const
{
    bool result = (m_Sheet == rhs.m_Sheet);

    if (result)
        result = (m_Flags[ColumnAddress] == rhs.m_Flags[ColumnAddress]);

    if (result && m_Flags[ColumnAddress])
        result = (m_Column == rhs.m_Column);

    if (result)
        result = (m_Flags[RowAddress] == rhs.m_Flags[RowAddress]);

    if (result && m_Flags[RowAddress])
        result = (m_Row == rhs.m_Row);

    return result;
}

bool ODSCellAddress::operator!=(const ODSCellAddress& rhs) const
{
    return !(*this == rhs);
}

bool ODSCellAddress::hasRow() const
{
    return m_Flags[RowAddress];
}

bool ODSCellAddress::hasColumn() const
{
    return m_Flags[ColumnAddress];
}

bool ODSCellAddress::absoluteRow() const
{
    return m_Flags[RowAbsAddress];
}

bool ODSCellAddress::absoluteColumn() const
{
    return m_Flags[ColumnAbsAddress];
}

std::wstring ODSCellAddress::quote(const std::wstring s) const
{
    wstring result = s;
    if ( (result.find(L'\'') != wstring::npos) ||
         (result.find(L' ') != wstring::npos) ||
         (result.find(L'\t') != wstring::npos) ) {
        wstring::size_type n = result.size();
        for (;n > 0;) {
            n--;
            if (result[n] == L'\'')
                result.replace(n, 1, L"\'\'");
        }
        result = L"\'" + result + L"\'";

    }
    return result;
}
