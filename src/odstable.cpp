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
#include "odstable.h"
#include "odsconst.h"
#include "xmldom.h"
#include "parser.h"
#include "odsformula.h"
#include <string>
#include <utility>

using namespace cppodsreport;
using namespace std;

#define ODSTABLE_CMD_V L"V"
#define ODSTABLE_CMD_H L"H"

ODSTable::ODSTable()
{    
}

ODSTable::ODSTable(ODSTable&& val):
    m_cellRanges(move(val.m_cellRanges)),
    m_table(move(val.m_table)),
    m_name(move(val.m_name))
{
    m_tableNode = val.m_tableNode;    
}

ODSTable::ODSTable(const DomElement &tableNode)
{    
    m_tableNode = tableNode;    
    m_name = DOMDocumentWrapper::DomStringToWstring(m_tableNode.attributeNS(ODS_NS_TABLE, ODS_ATTR_NAME));
}

void ODSTable::addNodeListToLastRow(const DomNodeList& list, ODSTableMap &tables)
{
    int cellCount = list.length();
    for (int j = 0; j < cellCount; ++j) {
        DomNode cell = list.item(j);
        if (cell.nodeType() == NODE_TYPE_ELEMENT) {
            m_table.back().cells().emplace_back(DOMDocumentWrapper::toElement(cell.cloneNode(true)));
            ODSFormulaSPtr f = m_table.back().cells().back().formula();
            if (f) {
                for (const ODSCellRangeSPtr& pr : f->cellRanges()) {
                    if (pr->begin.sheet().empty() || (pr->begin.sheet() == m_name)) {
                        m_cellRanges.emplace_back(pr.get());
                    }
                    else {
                        tables[pr->begin.sheet()].m_cellRanges.emplace_back(pr.get());
                    }
                }
            }
        }
    }
}

void ODSTable::parseTable(ODSTableMap &tables)
{
    DomNodeList list = m_tableNode.elementsByTagNameNS(ODS_NS_TABLE, ODS_ELEMENT_ROW);
    int rowCount = list.length();
    if (rowCount) {
        m_table.clear();
        for (int i = 0; i < rowCount; ++i) {
            DomElement row = DOMDocumentWrapper::toElement(list.item(i).cloneNode(false));
            m_table.emplace_back(row);                        
            DomNodeList cells = list.item(i).childNodes();
            addNodeListToLastRow(cells, tables);
        }
    }    
}

void ODSTable::save()
{
    DOMDocumentWrapper doc;
    int i;    

    DomNodeList list = m_tableNode.elementsByTagNameNS(ODS_NS_TABLE, ODS_ELEMENT_ROW);
    for (; list.length() > 0; ) {
        m_tableNode.removeChild(list.item(0));
        i = list.length();
    }

    for (ODSTableRow& row : m_table) {        
        DomNode rowNode = m_tableNode.appendChild(row.rowNode());        
        for(ODSCell& cell : row.cells()) {
            cell.save();
            rowNode.appendChild(cell.cellNode());
        }        
    }
}

void ODSTable::parseText(Table::iterator& row, ODSTableRow::ODSCellList::iterator& col, RowIndex& rowIndex, ColumnIndex& colIndex,
                              DataSource *ds)
{
    Parser* p = Parser::instance();

    wstring l, r, procesed;    
    wstring txt = col->textp();
    wstring text = txt;
    Parser::Args cmd;
    wstring var;
    bool res = false;    

    for(;!txt.empty();){
        cmd.clear();
        var.clear();
        res = p->parse(txt, l, r, cmd, var);

        procesed += l;
        txt = r;

        if (!cmd.empty()) {
            bool erased;
            procesed += processCommand(cmd, row, col, rowIndex, colIndex, ds, erased);
            if (erased)
                return;
        }
        if (!var.empty())
            procesed += processVariable(var, ds);
    }

    if (res) {
        if (text != procesed)
            col->setTextp(procesed);
    }    

    colIndex += col->repeatCount();
    ++col;
    if (col == row->cells().end()) {
        rowIndex += row->repeatCount();
        ++row;
        if (row != m_table.end()) {
            col = row->cells().begin();
            colIndex = 0;
        }
    }
}


void ODSTable::assignVars(DataSource* ds, ODSTableMap &tables)
{
    m_tableMap = &tables;
    RowIndex rindex = 0;
    ColumnIndex cindex = 0;
    Table::iterator i = m_table.begin();

    ODSTableRow::ODSCellList::iterator j;
    if (i != m_table.end())
        j = i->cells().begin();

    for ( ;i != m_table.end();) {
        parseText(i, j, rindex, cindex, ds);
    }
}

std::wstring ODSTable::processCommand(const Parser::Args& cmd, Table::iterator &row, ODSTableRow::ODSCellList::iterator& col,
                                      RowIndex& rowIndex, ColumnIndex& colIndex, DataSource *ds, bool &erased)
{
    bool ok = false;
    wstring result;
    wstring command;
    wstring ws;
    int w, h;

    erased = false;
    Parser::Args::const_iterator i = cmd.begin();
    ok = (i != cmd.end());
    if (ok) {
        command = *i;
        ok = (/*(command == ODSTABLE_CMD_H) ||*/ (command == ODSTABLE_CMD_V));
    }

    if (ok)
        ok = (++i != cmd.end());

    if (ok) {
        h = parseNumberStar(*i);
        ok = ((h >= 0) && (++i != cmd.end()));
    }

    if (ok) {
        w = parseNumberStar(*i);
        ok = ((w >= 0) && (++i != cmd.end()));
    }

    if (ok) {
        const wchar_t* str = i->c_str();
        if (ds->next(str)) {
            copyCells((command == ODSTABLE_CMD_V), row, col, rowIndex, colIndex, h, w);
        }
        else {
            removeCells((command == ODSTABLE_CMD_V), row, col, rowIndex, colIndex, h, w);
            erased = true;
        }
    }
    else {
        Parser *p = Parser::instance();
        result = p->commandBegin();
        i = cmd.begin();
        if (i != cmd.end())
            result += *i;
        for(++i; i != cmd.end(); ++i) {
            result += p->commandSeparator() + *i;
        }
        result += p->commandEnd();
    }

    return result;
}

int ODSTable::parseNumberStar(const std::wstring& ws)
{
    int result = -1;

    if (ws == L"*") {
        result = 0;
    }
    else {
        size_t pos;
        result = stoi(ws,&pos);
        if ((result < 1) || (pos < ws.size()))
            result = -1;
    }

    return result;
}

void ODSTable::copyCells(bool down, Table::iterator row, ODSTableRow::ODSCellList::iterator col, RowIndex rowIndex, ColumnIndex colIndex, int height, int width)
{    
    if (down) {
        copyCellsDown(row, col, rowIndex, colIndex, height, width);
    }
}

void ODSTable::copyCellsDown(Table::iterator row,  ODSTableRow::ODSCellList::iterator col, RowIndex rowIndex, ColumnIndex colIndex, unsigned int height, unsigned int width)
{    
    Table::iterator last =  row;
    height = rowRange(height, row, last);

    if (!height)
        return;

    Table::iterator i = last;
    advance(i,1);
    unsigned int rowCount = 0;
    updateRowInCellRanges(rowIndex + height, height);
    do {
        Table::iterator j = m_table.insert(i, ODSTableRow(DOMDocumentWrapper().toElement(row->rowNode().cloneNode(true))));
        if (rowCount + j->repeatCount() > height) {
            j->setRepeatCount(height - rowCount);
        }
        rowCount += j->repeatCount();
        DomDocument doc = m_tableNode.ownerDocument();
        if (0 != colIndex)
            j->cells().insert(j->cells().end(), colIndex, ODSCell(doc));
        row->copyCells(*j, j->cells().end(), colIndex, width);
        ++row;
        for (ODSCell &c : j->cells()) {
            updateRowInCellRange(c, rowIndex, height);
        }
    }while (rowCount < height);
}

void ODSTable::removeCells(bool down, Table::iterator& row, ODSTableRow::ODSCellList::iterator& col, RowIndex& rowIndex, ColumnIndex& colIndex, int height, int width)
{
    if (down) {
        removeRows(row, col, rowIndex, colIndex, height);
    }
}

void ODSTable::removeRows(Table::iterator& row, ODSTableRow::ODSCellList::iterator& col, RowIndex &rowIndex, ColumnIndex& colIndex, int height)
{
    Table::iterator last;
    height = rowRange(height, row, last);
    if (0 != height) {
        ++last;
        m_table.erase(row, last);
        row = last;
        if (row != m_table.end()) {
            col = row->cells().begin();
            colIndex = 0;
        }
        updateRowInCellRanges(rowIndex, -height);
    }
}

std::wstring ODSTable::processVariable(const wstring &var, DataSource* ds)
{
    wstring result;
    bool ok = false;
    int sz = 256;
    unique_ptr<wchar_t[]> buf;

    for (int i = 0; !ok && (i < 2) && (sz != 0); ++i) {
        buf.reset(new wchar_t[sz]);
        ok = ds->value(var.c_str(), buf.get(), &sz);
    }

    if (ok) {
        result = wstring(buf.get());
    }
    else {
        result = Parser::instance()->variableBegin() + var + Parser::instance()->variableEnd();
    }

    return result;
}

int ODSTable::rowRange(int height, Table::iterator first, Table::iterator& last)
{
    int rowCount = 0;
    if (first == m_table.end())
        return rowCount;

    if (height == 0) {
        for (last = first; last != m_table.end(); ++last) {
            rowCount += last->repeatCount();
        }
        --last;
    }
    else {
        for (last = first; last != m_table.end(); ++last) {
            rowCount += last->repeatCount();
            if (rowCount >= height) {
                //split row
                if (rowCount > height) {
                    unsigned int delta = rowCount - height;
                    Table::iterator i = last;
                    ++i;
                    i = m_table.insert(i, ODSTableRow());
                    *i = *last;                    
                    i->setRepeatCount(delta);
                    last->setRepeatCount(last->repeatCount() - delta);
                }

                rowCount = height;
                break;
            }
        }
        if (last == m_table.end())
            --last;
    }

    return rowCount;
}

std::wstring ODSTable::name() const
{
    return m_name;
}

void ODSTable::updateRowInCellRanges(RowIndex startRow, int updateValue)
{
    for(ODSCellRange* &r : m_cellRanges) {
        updateRowInCellRange(r, startRow, updateValue);
    }
}

void ODSTable::updateRowInCellRange(ODSCell& cell, RowIndex startRow, int updateValue)
{
    if (cell.formula()) {
        for (const ODSCellRangeSPtr &r : cell.formula()->cellRanges()) {
            if (r) {
                if (r->begin.sheet().empty() || (r->begin.sheet() == m_name)) {
                    updateRowInCellRange(r.get(), startRow, updateValue);
                    m_cellRanges.emplace_back(r.get());
                }
                else {
                    (*m_tableMap)[r->begin.sheet()].m_cellRanges.emplace_back(r.get());
                }
            }
        }
    }
}

void ODSTable::updateRowInCellRange(ODSCellRange* r, RowIndex startRow, int updateValue)
{
    if (r) {
        if (r->begin.hasRow() && !r->begin.absoluteRow() && (r->begin.row() >= startRow) ) {
            r->begin.setRow(r->begin.row() + updateValue);
        }
        if (r->end.hasRow() && !r->end.absoluteRow() && (r->end.row() >= startRow) ) {
            r->end.setRow(r->end.row() + updateValue);
        }
    }
}
