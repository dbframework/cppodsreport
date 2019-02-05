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
#include "repeateditems.h"

namespace cppodsreport {

class ODSTableRow
{
public:
    //typedef std::list<ODSCell> ODSCellList;
    typedef RepeatedItems<ODSCell, DomDocument&> ODSCellList;
private:
    DomElement m_rowNode;
    ODSCellList m_cells;
    size_t m_repeatCount;    
public:    
    ODSTableRow() = delete;
    ODSTableRow(DomDocument& doc);
    ODSTableRow(DomDocument& doc, const DomElement& rowNode);
    ODSTableRow(const ODSTableRow&);
    ODSTableRow& operator=(const ODSTableRow&);
    ODSCellList& cells();
    DomElement rowNode();
    /*!
        This methos copies cells from this ODSTableRow to another ODSTableRow inserting them to the specified position.
        @param[in] destRow ODSTableRow to copy to.
        @param[in] where destRow iterator specifying position where to insert copied cells.
        @param[in] index Position in this ODSTableRow to copy from (cell zero-based index). 
        @param[in] count Number of cells to copy. If it is zero all cells up to the end of the row will be copied.
    */
    void copyCells(ODSTableRow& destRow, ODSCellList::iterator where, ODSCellList::size_type index, ODSCellList::size_type count);
    size_t repeatCount() const;
    void setRepeatCount(size_t value);    
};

}
#endif // ODSTABLEROW_H
