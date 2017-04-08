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
#ifndef ODSFORMULA_H
#define ODSFORMULA_H

#include <string>
#include <memory>
#include <bitset>
#include <deque>
#include "odscelladdress.h"
#include "odscellrange.h"

namespace cppodsreport {

class ODSFormula
{
public:
    typedef std::deque<ODSCellRangeSPtr> CellRanges;
private:
    std::wstring m_formula;
    void parse();
    typedef std::wstring::size_type Index;

    Index processReference(Index i);
    void parseReference(Index begin, Index end);

    CellRanges m_cellRanges;
    void assign(const ODSFormula& rhs);
public:    
    ODSFormula();
    ODSFormula(std::wstring f);
    ODSFormula(const ODSFormula& f);
    std::wstring formula() const;
    void save();
    CellRanges& cellRanges();
    const CellRanges& cellRanges() const;

    ODSFormula& operator=(const ODSFormula& rhs);
};

typedef std::shared_ptr<ODSFormula> ODSFormulaSPtr;

};//namespace

#endif // ODSFORMULA_H
