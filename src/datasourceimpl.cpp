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
#include "datasourceimpl.h"
#include <cwchar>

using namespace cppodsreport;
using namespace std;

DataSourceCell::DataSourceCell()
{
    m_nestedTable = nullptr;
}

void DataSourceCell::clear()
{
    if (m_nestedTable != nullptr) {
        delete m_nestedTable;
        m_nestedTable = nullptr;
    }
}

DataSourceCell::~DataSourceCell()
{
    clear();
}

std::wstring DataSourceCell::value()
{
    return m_value;
}

void DataSourceCell::setValue(const std::wstring& str)
{
    m_value = str;
    clear();
}

DataSourceTable* DataSourceCell::nested()
{
    return m_nestedTable;
}

void DataSourceCell::makeNested()
{
    m_value.clear();
    if (m_nestedTable == nullptr) {
        m_nestedTable = new DataSourceTable(true);
    }
}

DataSourceTable::DataSourceTable(bool initialEmpty)
{
    m_state = (initialEmpty ? PCR_BOF : PCR_RECORD);
    if (initialEmpty) {
        m_PCR = m_rows.end();
    }
    else {
        addRow();
        m_PCR = m_rows.begin();
    }
}

void DataSourceTable::split(const std::wstring& str, std::wstring& arg1, std::wstring& arg2)
{
    wstring::size_type pos = str.find('.');
    if (pos == wstring::npos) {
        arg1 = str;
        arg2.clear();
    }
    else {
        arg1 = str.substr(0, pos);
        arg2 = str.substr(pos + 1);
    }
}

void DataSourceTable::addRow()
{
    DataSourceRow row;
    row.reserve(m_columns.size());
    m_rows.push_back(row);    
}

void DataSourceTable::addColumn(const std::wstring& columnName)
{
    m_columns.insert(Columns::value_type(columnName, m_columns.size()));
}

DataSourceCell* DataSourceTable::cell(const wstring &columnName)
{
    if (m_rows.empty()) {
        return nullptr;
    }
    else {
        return cell(m_rows.back(), columnName);
    }
}

DataSourceCell* DataSourceTable::cell(DataSourceRow &row, const std::wstring& columnName)
{
    DataSourceCell* result = nullptr;

    Columns::iterator col = m_columns.find(columnName);
    if (col == m_columns.end())
        return result;

    DataSourceRow::size_type index = col->second;
    if (index >= row.size())
        row.insert(row.end(), index - row.size() + 1, DataSourceCell());

    result = &row[index];
    return result;
}

bool DataSourceTable::next(const std::wstring& columnName)
{
    bool result = false;

    if (columnName.empty()) {//this table
        if (m_state == PCR_BOF) {
            if (m_rows.empty()) {
                result = false;
            }
            else {
                m_PCR = m_rows.begin();
                m_state = PCR_RECORD;
                result = true;
            }
        }
        else if (m_state == PCR_EOF) {
            result = false;
        }
        else {
            ++m_PCR;
            if (m_PCR == m_rows.end()) {
                m_state = PCR_EOF;
                result = false;
            }
            else {
                result = true;
            }
        }
    }
    else {//nested table
        wstring l, r;

        if ((m_state == PCR_BOF) || (m_state == PCR_EOF)) {
            result = false;
        }
        else {
            split(columnName, l, r);
            DataSourceCell* c = cell(*m_PCR, l);
            if (c != nullptr) {
                if (c->nested() != nullptr) {
                    result = c->nested()->next(r);
                }
            }
        }

    }

    return result;
}

bool DataSourceTable::value(const std::wstring& columnName, std::wstring& cellValue)
{
    bool result = false;

    if ((m_state == PCR_BOF) || (m_state == PCR_EOF))
        return result;

    wstring l, r;
    split(columnName, l, r);
    DataSourceCell *c = cell(*m_PCR, l);
    if (c != nullptr) {
        if ((c->nested() != nullptr) && !r.empty())
            result = c->nested()->value(r, cellValue);
        if((c->nested() == nullptr) && r.empty()) {
            result = true;
            cellValue = c->value();
        }
    }

    return result;
}


DataSourceImpl::DataSourceImpl()
{
}

DataSourceTable *DataSourceImpl::addTable(const std::wstring& tableName, bool initialStateBOF)
{
    pair<Tables::iterator, bool> p = m_tables.emplace(tableName, initialStateBOF);    
    return (p.second ? &p.first->second : nullptr);
}

DataSourceTable* DataSourceImpl::table(const std::wstring& tableName)
{
    DataSourceTable* result = nullptr;
    Tables::iterator i = m_tables.find(tableName);
    if (i != m_tables.end())
        result = &i->second;
    return result;
}

bool DataSourceImpl::next(const wchar_t *tblName)
{
    if (tblName == nullptr)
        return false;

    wstring l, r;
    wstring tableName(tblName);
    bool result = false;
    
    DataSourceTable::split(tableName, l, r);
    DataSourceTable* t = table(l);
    if (t != nullptr) {
        result = t->next(r);
    }
    return result;
}

bool DataSourceImpl::value(const wchar_t* columnName, wchar_t* cellValue, int* size)
{
    if ((columnName == nullptr) || (cellValue == nullptr) || (size == nullptr))
        return false;

    wstring l, r;
    bool result = false;
    wstring val;
    DataSourceTable::split(wstring(columnName), l, r);
    DataSourceTable* t = table(l);
    if (t != nullptr) {
        result = t->value(r, val);
        if (result) {
            if (*size > val.size()) {
                wcscpy(cellValue, val.c_str());
            }
            else {                
                result = false;
            }
            *size = val.size() + 1;
        }        
    }

    return result;
}


