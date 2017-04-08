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
#ifndef DATASOURCEIMPL_H
#define DATASOURCEIMPL_H

#include "datasourceabstract.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <list>
#include <vector>

namespace cppodsreport {

class DataSourceTable;

class DataSourceCell {
private:
    std::wstring m_value;    
    DataSourceTable* m_nestedTable;
    void clear();
public:
    DataSourceCell();
    ~DataSourceCell();
    std::wstring value();
    void setValue(const std::wstring& str);
    DataSourceTable* nested();
    void makeNested();
};

class DataSourceTable {
friend class DataSourceImpl;
private:
    typedef std::vector<DataSourceCell> DataSourceRow;
    typedef std::list<DataSourceRow> Rows;    
    Rows m_rows;
    enum State {
        PCR_BOF,
        PCR_RECORD,
        PCR_EOF
    };
    State m_state;
	typedef std::unordered_map<std::wstring, DataSourceRow::size_type> Columns;
	Columns m_columns;    
    Rows::iterator m_PCR;
    DataSourceTable(){};    
    DataSourceCell* cell(DataSourceRow& row, const std::wstring& columnName);
    static void split(const std::wstring& str, std::wstring& arg1, std::wstring& arg2);
public:	
    DataSourceTable(bool initialEmpty);    
    void addRow();
    void addColumn(const std::wstring& columnName);    
    bool next(const std::wstring& columnName);
    bool value(const std::wstring& columnName, std::wstring& cellValue);
    DataSourceCell* cell(const std::wstring& columnName);
};

class DataSourceImpl : public DataSource
{
private:
    typedef std::unordered_map<std::wstring, DataSourceTable> Tables;
    Tables m_tables;
public:
    DataSourceImpl();    
    bool next(const wchar_t* tblName);
    bool value(const wchar_t* columnName, wchar_t* cellValue, int* size);
    DataSourceTable* addTable(const std::wstring& tableName, bool initialStateBOF);
    DataSourceTable* table(const std::wstring& tableName);
};

}

#endif // DATASOURCEIMPL_H
