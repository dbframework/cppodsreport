/*
Copyright (c) 2019 Sidorov Dmitry

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

#ifndef CPPODSREPORTCORE_H
#define CPPODSREPORTCORE_H

#include "cppodsreportexpdef.h"
#include "datasourceabstract.h"

namespace cppodsreport {

typedef size_t ODSSize;

class ODSSheetCell {
public:
    virtual const wchar_t* text() const = 0;
    virtual void setText(const wchar_t* text) = 0;
};

class ODSSheet {
public:
    virtual ODSSheetCell& cell(ODSSize row, ODSSize col) = 0;
    virtual const wchar_t* sheetName() const = 0;
    virtual void setSheetName(const wchar_t* name) = 0;
};

class ODFPackage {
public:      
    virtual bool open(const char* fileName) = 0;
    virtual bool save(const char* fileName) = 0;
#ifdef CPPODSREPORT_WIN
    virtual bool opena(const char* fileName) = 0;
    virtual bool open(const wchar_t* fileName) = 0;
    virtual bool savea(const char* fileName) = 0;
    virtual bool save(const wchar_t* fileName) = 0;
#endif
    virtual bool process(DataSource* ds) = 0;
    virtual bool save() = 0;
    virtual bool close() = 0;    
    
    virtual ODSSize sheetCount() = 0;
    virtual ODSSheet& sheet(ODSSize sheetIndex) = 0;
    virtual ODSSize appendSheet() = 0;
};


}//namespace

#endif //CPPODSREPORTCORE_H
