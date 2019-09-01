/*
Copyright (c) 2017-2019 Sidorov Dmitry

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
#include "cppodsreportcore.h"
#include "repeateditems.h"

namespace cppodsreport {

class ODSFile;

class ODSTable : public ODSSheet
{
public:
    //typedef std::unordered_map<std::wstring, ODSTable> ODSTableMap;
    typedef std::vector<ODSTable> ODSTableVector;
    typedef std::unordered_map<std::wstring, ODSTableVector::size_type> ODSTableMap;    
private:
    DomElement m_tableNode;    
    //typedef std::list<ODSTableRow> Table;
    typedef RepeatedItems<ODSTableRow, DomDocument&> Table;
    Table m_table;
    std::wstring m_name;
    ODSTableMap* m_tableMap;
    ODSTableVector* m_tableVector;       
    ODSFile* m_file;

    typedef std::vector<ODSCellRange*> CellRangeVector;
    CellRangeVector m_cellRanges;

    ODSTable& tableByName(const std::wstring& name);

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
    void addNodeListToLastRow(const DomNodeList& list);
    void updateRowInCellRanges(RowIndex startRow, int updateValue);
    void updateRowInCellRange(ODSCell& cell, RowIndex startRow, int updateValue);
    void updateRowInCellRange(ODSCellRange *r, RowIndex startRow, int updateValue);
    void createNode();
public:    
    ODSTable() = delete;
    ODSTable(ODSFile* file);
    ODSTable(ODSTable&& val);
    ODSTable(ODSFile* file, const DomElement& tableNode);    
    void parseTable(ODSTableMap& tables, ODSTableVector& vector);
    void assignVars(DataSource* ds, ODSTableMap& tables, ODSTableVector& vector);
    void save();
    std::wstring name() const;
    void setName(const std::wstring& name);
    ODSSheetCell& cell(ODSSize row, ODSSize col);
    const wchar_t* sheetName() const;
    void setSheetName(const wchar_t* name);
};



}

#endif // ODSTABLE_H
