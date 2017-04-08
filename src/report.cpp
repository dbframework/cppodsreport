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
#include "xmldom.h"
#include "cppodsreport.h"
#include "domtypes.h"
#include <iostream>
#include <fstream>

using namespace cppodsreport;
using namespace std;

#define DATA_NAMESPACE "urn:cppodsreport:report:1.0"

#define EL_DATASOURCE "DataSource"

#define EL_VALUE L"Value"
#define EL_TABLE L"Table"
#define EL_COLUMN L"Column"

#define AT_NAME "Name"
#define AT_BLOCK "BlockTable"
#define AT_COLNAME "ColumnName"

#ifdef CPPODSREPORT_WIN
#define MAIN_NAME wmain
#define MAIN_ARG_TYPE wchar_t
#define REPORT_FUNC reportw
#else
#define MAIN_NAME main
#define MAIN_ARG_TYPE char
#define REPORT_FUNC report
#endif //CPPODSREPORT_WIN

bool readFile(MAIN_ARG_TYPE* fileName, unique_ptr<char[]>& buf, size_t& length)
{
    bool result = false;
    ifstream f;
    f.open(fileName, ios_base::in | ios_base::binary);
    if (f.good()) {
        f.seekg (0, ifstream::end);
        length = f.tellg();
        f.seekg (0, ifstream::beg);

        if ((length > 0) && f.good()) {
            buf.reset(new char[length]);
            f.read(buf.get(), length);
            result = f.good();
        }
    }

    return result;
}

void processColumn(DataSourceTable* table, DomElement& column)
{
	wstring name = DOMDocumentWrapper::DomStringToWstring(column.getAttributeNS(DATA_NAMESPACE, AT_NAME));
    if (!name.empty())
        table->addColumn(name);		
}


void processNestedTable(DataSourceTable* t, DomElement& table, bool isBlock);

void processCell(DataSourceTable* table, DomElement& cell)
{
    wstring colName = DOMDocumentWrapper::DomStringToWstring(cell.attributeNS(DATA_NAMESPACE, AT_COLNAME));
    DataSourceCell* c = table->cell(colName);
    if (c == nullptr)
        return;

    DomNodeList childNodes = cell.childNodes();
    if (childNodes.length() > 0) {
        DomNode n = childNodes.item(0);
        if (n.nodeType() == NODE_TYPE_ELEMENT) {
            DomElement el = DOMDocumentWrapper::toElement(n);
            wstring localName = DOMDocumentWrapper::DomStringToWstring(el.localName());
            if (localName == EL_VALUE) {
                DomNodeList children = el.childNodes();
                wstring val;
                for (int i = 0; i < children.length(); ++i) {
                    if (children.item(i).nodeType() == NODE_TYPE_TEXT) {
                        val = DOMDocumentWrapper::DomStringToWstring(children.item(i).nodeValue());
                        break;
                    }
                }
                c->setValue(val);
            }
            else {
                c->makeNested();
                processNestedTable(c->nested(), el, true);
            }
        }
    }
}

void processRow(DataSourceTable* table, DomElement& row)
{
    DomNodeList cells = row.childNodes();
    for (int i = 0; i < cells.length(); ++i) {
        if (cells.item(i).nodeType() == NODE_TYPE_ELEMENT) {
            DomElement el = DOMDocumentWrapper::toElement(cells.item(i));
            processCell(table, el);
        }
    }
}

void processNestedTable(DataSourceTable* t, DomElement& table, bool isBlock)
{
    DomNodeList childNodes = table.childNodes();
    for (int i = 0; i < childNodes.length(); ++i) {
        DomNode n = childNodes.item(i);
        if (n.nodeType() == NODE_TYPE_ELEMENT) {
            DomElement el = DOMDocumentWrapper::toElement(n);
            wstring localName = DOMDocumentWrapper::DomStringToWstring(el.localName());
            if (localName == EL_COLUMN) {
                processColumn(t, el);
            }
            else  {
                if (isBlock)
                    t->addRow();
                processRow(t, el);
            }
        }
    }
}


void processTable(DataSourceImpl& ds, DomElement& table)
{
    wstring name = DOMDocumentWrapper::DomStringToWstring(table.attributeNS(DATA_NAMESPACE, AT_NAME));
    wstring isBlock = DOMDocumentWrapper::DomStringToWstring(table.attributeNS(DATA_NAMESPACE, AT_BLOCK));
    bool block = ((isBlock == L"true") || (isBlock == L"1"));
    DataSourceTable* t = ds.addTable(name, block);
    if (t != nullptr) {
        processNestedTable(t, table, block);
    }
}


void fillDataSource(DOMDocumentWrapper& wrapper, DataSourceImpl& ds)
{
    DomNode doc = wrapper.document().documentElement();
    DomNodeList tables = doc.childNodes();
    for (int i = 0; i < tables.length(); ++i) {
        DomNode n = tables.item(i);
        if (n.nodeType() == NODE_TYPE_ELEMENT) {
            DomElement el = wrapper.toElement(n);
            processTable(ds, el);
        }
    }    
}

int MAIN_NAME(int argc, MAIN_ARG_TYPE *argv[])
{
    if (argc < 3) {
        wcout << L"Usage: report <data file name> <template file name>" << endl << endl;
        wcout << L"Report utility generates report by substituting values from a data file instead of variables in a template file." << endl;
        wcout << L"Data file is a xml file of special format containing data for report." << endl;
        wcout << L"Template file is a *.ods file containing report template." << endl;
        wcout << L"See documentation for details." << endl;
		return 1;
    }

    unique_ptr<char[]> buf;
    size_t len;

    if (readFile(argv[1], buf, len)) {
        DOMDocumentWrapper wrapper;

        if (wrapper.setContent(buf.get(), len)) {
            DataSourceImpl ds;
            fillDataSource(wrapper, ds);
            int internal, zip, zlib, sys;
			bool result = REPORT_FUNC(argv[2], &ds, &internal, &zip, &zlib, &sys);
            if (result) {
                wcout << L"Report created." << endl;
				return 0;
            }
            else {
                wcout << L"Error creating report." << endl;                
                wcout << L"Internal error: " << internal << endl;
                wcout << L"Zip error: " << zip << endl;
                wcout << L"Zlib error: " << zlib << endl;
                wcout << L"System error (errno): " << sys << endl;
            }

        }
        else {
            wcout << L"Error parsing data file." << endl;
        }
    }
    else {
        wcout << L"Error reading data file." << endl;
    }

	return 2;
}

