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
#ifndef ODSTABLEROW_H
#define ODSTABLEROW_H

#include "xmldom.h"
#include "odscell.h"
#include <list>

namespace cppodsreport {

class ODSTableRow
{
public:
    typedef std::list<ODSCell> ODSCellList;
private:
    DomElement m_rowNode;
    ODSCellList m_cells;
    unsigned int m_repeatCount;    
public:    
    ODSTableRow();
    ODSTableRow(DomDocument& doc);
    ODSTableRow(const DomElement& rowNode);
    ODSCellList& cells();
    DomElement rowNode();
    void copyCells(ODSTableRow& targetRow, ODSCellList::iterator where, ODSCellList::size_type index, ODSCellList::size_type count);
    unsigned int repeatCount() const;
    void setRepeatCount(unsigned int value);
    //void copyTo(ODSTableRow& copy);
};

}
#endif // ODSTABLEROW_H
