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
#include "odscellrange.h"

using namespace cppodsreport;
using namespace std;

ODSCellRange::ODSCellRange()
{
    m_beginPos = 0;
    m_size = 0;
}

bool ODSCellRange::setValue(const std::wstring& str)
{
    bool result = false;
    if (str.size() < 2)
        return result;

    //don't process Reference Error
    if (str[1] == L'#')
        return result;

    //don't process ranges with IRI part
    if (str.find(L'#') != wstring::npos)
        return result;

    Index middle = str.find(L':');
    if (middle == wstring::npos) {
        if (begin.setValue(str.substr(1, str.size() - 2)))
            return result;
        end = begin;
    }
    else {
        if (!begin.setValue(str.substr(1, middle - 1)))
            return result;
        if (!end.setValue(str.substr(middle + 1, str.size() - middle - 2)))
            return result;
        if (begin.sheet() != end.sheet()) {
            if (end.sheet().empty()) {
                end.setSheet(begin.sheet());
            }
            else {
                return result;
            }
        }
    }

    result = true;

    return result;
}

std::wstring ODSCellRange::toWString() const
{
    wstring result = L"[" + begin.toWString();
    if (begin != end) {
        result += L":" + end.toWString();
    }
    result += L"]";
    return result;
}

