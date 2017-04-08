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
#include "odsformula.h"


using namespace cppodsreport;
using namespace std;

ODSFormula::ODSFormula()
{
}

ODSFormula::ODSFormula(std::wstring f):
    m_formula(f)
{
    parse();
}

void ODSFormula::parse()
{
    m_cellRanges.clear();
    for(wstring::size_type i = 0; i != wstring::npos;) {
        i = processReference(i);
    }
}

ODSFormula::Index ODSFormula::processReference(Index i)
{
    Index begin = m_formula.find(L'[', i);
    Index end;
    Index result = wstring::npos;
    if (begin != wstring::npos) {
        end = m_formula.find(L']', begin + 1);
        if (end != wstring::npos) {
            parseReference(begin, end);
            if (end < m_formula.size() - 1)
                result = end +1;
        }
    }
    return result;
}

void ODSFormula::parseReference(Index begin, Index end)
{    
    wstring reference = m_formula.substr(begin, end - begin + 1);

    unique_ptr<ODSCellRange> r(new ODSCellRange);

    if (r->setValue(reference)) {
        r->m_beginPos = begin;
        r->m_size = end - begin + 1;
        m_cellRanges.emplace_front(r.release());
    }
}


std::wstring ODSFormula::formula() const
{
    return m_formula;
}

void ODSFormula::save()
{
    for (const ODSCellRangeSPtr& cr : m_cellRanges) {
        m_formula.replace(cr->m_beginPos, cr->m_size, cr->toWString());
    }
}

ODSFormula::CellRanges& ODSFormula::cellRanges()
{
    return m_cellRanges;
}

const ODSFormula::CellRanges& ODSFormula::cellRanges() const
{
    return m_cellRanges;
}

void ODSFormula::assign(const ODSFormula& rhs)
{
    m_formula = rhs.formula();
    m_cellRanges.clear();
    for (const ODSCellRangeSPtr& r :rhs.cellRanges()) {
        m_cellRanges.emplace_back(new ODSCellRange(*r));
    }
}

ODSFormula& ODSFormula::operator=(const ODSFormula& rhs)
{
    if (this != &rhs)
        assign(rhs);
    return *this;
}

ODSFormula::ODSFormula(const ODSFormula& f)
{
    assign(f);
}
