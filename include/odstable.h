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
#ifndef ODSTABLE_H
#define ODSTABLE_H

#include "xmldom.h"
#include "odstablerow.h"
#include <list>
#include <string>
#include <vector>
#include <unordered_map>
#include "datasourceabstract.h"
#include "parser.h"
#include "odscellrange.h"

namespace cppodsreport {


class ODSTable
{
public:
    typedef std::unordered_map<std::wstring, ODSTable> ODSTableMap;
private:
    DomElement m_tableNode;    
    typedef std::list<ODSTableRow> Table;
    Table m_table;
    std::wstring m_name;
    ODSTableMap* m_tableMap;

    typedef std::vector<ODSCellRange*> CellRangeVector;
    CellRangeVector m_cellRanges;

    typedef Table::size_type RowIndex;
    typedef ODSTableRow::ODSCellList::size_type ColumnIndex;    

    void parseText(Table::iterator& row, ODSTableRow::ODSCellList::iterator& col, RowIndex& rowIndex, ColumnIndex& colIndex,
                        DataSource* ds);
    std::wstring processCommand(const cppodsreport::Parser::Args &cmd, Table::iterator& row, ODSTableRow::ODSCellList::iterator& col,
                                RowIndex& rowIndex, ColumnIndex& colIndex, DataSource* ds, bool& erased);
    std::wstring processVariable(const std::wstring &var, DataSource* ds);
    int parseNumberStar(const std::wstring& ws);
    void copyCells(bool down, Table::iterator row, ODSTableRow::ODSCellList::iterator col, RowIndex rowIndex, ColumnIndex colIndex, int height, int width);
    void copyCellsDown(Table::iterator row, ODSTableRow::ODSCellList::iterator col, RowIndex rowIndex, ColumnIndex colIndex, unsigned int height, unsigned int width);
    void removeCells(bool down, Table::iterator& row, ODSTableRow::ODSCellList::iterator& col, RowIndex& rowIndex, ColumnIndex& colIndex, int height, int width);
    void removeRows(Table::iterator& row, ODSTableRow::ODSCellList::iterator& col, RowIndex& rowIndex, ColumnIndex& colIndex, int height);
    int rowRange(int height, Table::iterator first, Table::iterator& last);
    void addNodeListToLastRow(const DomNodeList& list, ODSTableMap& tables);
    void updateRowInCellRanges(RowIndex startRow, int updateValue);
    void updateRowInCellRange(ODSCell& cell, RowIndex startRow, int updateValue);
    void updateRowInCellRange(ODSCellRange *r, RowIndex startRow, int updateValue);
public:    
    ODSTable();
    ODSTable(ODSTable&& val);
    ODSTable(const DomElement& tableNode);    
    void parseTable(ODSTableMap& tables);
    void assignVars(DataSource* ds, ODSTableMap& tables);
    void save();
    std::wstring name() const;

};



}

#endif // ODSTABLE_H
