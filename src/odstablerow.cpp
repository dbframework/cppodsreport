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
#include "odstablerow.h"
#include "odsconst.h"

using namespace cppodsreport;
using namespace std;

ODSTableRow::ODSTableRow()
{
    m_repeatCount = 1;
}

ODSTableRow::ODSTableRow(DomDocument& doc)
{
    m_repeatCount = 1;
    m_rowNode = doc.createElementNS(ODS_NS_TABLE, DOMDocumentWrapper::qualifiedName(ODS_NSP_TABLE, ODS_ELEMENT_ROW));
}

ODSTableRow::ODSTableRow(const DomElement& rowNode)
{
    m_repeatCount = 1;
    m_rowNode = rowNode;
    DomString s = m_rowNode.getAttributeNS(ODS_NS_TABLE, ODS_ATTR_ROW_REP);
    wstring ws = DOMDocumentWrapper().DomStringToWstring(s);
    if (!ws.empty()) {
        size_t idx;
        m_repeatCount = stoul(ws, &idx);
        if (idx < ws.size())
            m_repeatCount = 1;
    }
}

ODSTableRow::ODSCellList &ODSTableRow::cells()
{
    return m_cells;
}

DomElement ODSTableRow::rowNode()
{
    return m_rowNode;
}

void ODSTableRow::copyCells(ODSTableRow &destRow, ODSCellList::iterator where, ODSCellList::size_type index, ODSCellList::size_type count)
{
     ODSCellList::iterator i = m_cells.begin();
     ODSCellList::size_type pos = 0;
     size_t toCopy;
     ODSCellList::size_type totalCellsToCopy  = count;  

     //Every cell can represent several identical cells depending on ODSCell.m_repeatCount.
     //Code below finds which cell represents cell in position <index>.
     for (; (i != m_cells.end()) && (pos < index); ++i) {
         if (pos + i->repeatCount() > index)
             break;
         pos += i->repeatCount();
     }

     //Copy cells to the end of the row or until totalCellsToCopy cells copied.
     for(; (i != m_cells.end()) && ((count == 0) || (totalCellsToCopy > 0)); ++i) {
         //toCopy defines how many times we need to copy current cell. On first and last iteration 
         //only part of the cells, represented by current cell, may be required to copy.
         //Next line represents possible split of the first cell.
         toCopy = i->repeatCount() - (index - pos);
         //Next line represents possible split of the last cell.
         if (totalCellsToCopy > 0)
             toCopy = min(toCopy, totalCellsToCopy);
         //To make (index - pos) zero after first iteration.
         pos = index;         
         ODSCell c(*i);
         if (toCopy != c.repeatCount()) {
             c.setRepeatCount(toCopy);
         }
         destRow.cells().insert(where, c);
         totalCellsToCopy -= toCopy;
     }
}

unsigned int ODSTableRow::repeatCount() const
{
    return m_repeatCount;
}

void ODSTableRow::setRepeatCount(unsigned int value)
{
    m_repeatCount = value;
    wstring s = to_wstring(m_repeatCount);
    m_rowNode.setAttributeNS(ODS_NS_TABLE, DOMDocumentWrapper::qualifiedName(ODS_NSP_TABLE, ODS_ATTR_ROW_REP), DOMDocumentWrapper().WstringToDomString(s));
}

