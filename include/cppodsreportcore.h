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

/*!
    Integer type used by library methods.
*/
typedef size_t ODSSize;


/*!
    The ODSSheetCell is an abstract class used to manipulate spreadsheet cell. 
*/
class ODSSheetCell {
public:
    /*!
        Get cell value as text.
        @return Cell value.
    */
    virtual const wchar_t* text() const = 0;
    /*!
        Set cell value as text.
        @param[in] text New cell value.
    */
    virtual void setText(const wchar_t* text) = 0;
};

/*!
    The ODSSheet is an abstract class used to manipulate sheet. 
*/
class ODSSheet {
public:
    /*!
        Access sheet cell by zero-based indexes.
        @param[in] row Sheet row (zero-based).
        @param[in] col Sheet column (zero-based).
        @return Reference to ODSSheetCell instance.
    */
    virtual ODSSheetCell& cell(ODSSize row, ODSSize col) = 0;
    /*!
        Get sheet name.
        @return Sheet name.
    */
    virtual const wchar_t* sheetName() const = 0;
    /*!
        Set sheet name.
        @param[in] name Sheet name.
    */
    virtual void setSheetName(const wchar_t* name) = 0;
};

/*!
    The ODFPackage is an abstract class used to manipulate ODF spreadsheet file. 
*/
class ODFPackage {
public: 
    /*!
        Open ODF spreadsheet file.
        @param[in] fileName File name in UTF-8 encoding.
        @return True if success.
    */
    virtual bool open(const char* fileName) = 0;
    /*!
        Save ODF spreadsheet file using provided name.
        @param[in] fileName New file name in UTF-8 encoding.
        @return True if success.
    */
    virtual bool save(const char* fileName) = 0;
#ifdef CPPODSREPORT_WIN
    /*!
        Open ODF spreadsheet file. Available only for Windows platform.
        @param[in] fileName File name in ANSI (Windows) encoding.
        @return True if success.
    */
    virtual bool opena(const char* fileName) = 0;
    /*!
        Open ODF spreadsheet file. Available only for Windows platform.
        @param[in] fileName File name in UTF-16 encoding.
        @return True if success.
    */
    virtual bool open(const wchar_t* fileName) = 0;
    /*!
        Save ODF spreadsheet file using provided name. Available only for Windows platform.
        @param[in] fileName New file name in ANSI (Windows) encoding.
        @return True if success.
    */
    virtual bool savea(const char* fileName) = 0;
    /*!
        Save ODF spreadsheet file using provided name. Available only for Windows platform.
        @param[in] fileName New file name in UTF-16 encoding.
        @return True if success.
    */
    virtual bool save(const wchar_t* fileName) = 0;
#endif
    /*!
        Substitute variables with data values from provided DataSource.
        @param[in] ds Pointer to DataSource instance.
        @return True if success.
    */
    virtual bool process(DataSource* ds) = 0;
    /*!
        Save ODF spreadsheet file using current name. File must be previously opened.
        @return True if success.
    */
    virtual bool save() = 0;
    /*!
        Save ODF spreadsheet file using current name and close it. File must be previously opened. After call
        to this method file is closed even it wasn't saved successfully.
        @return True if file is saved successfully, false otherwise. 
    */
    virtual bool close() = 0;    
    /*!
        Get the number of sheets in the ODF spreadsheet file.
        @return Sheet number.
    */
    virtual ODSSize sheetCount() = 0;
    /*!
        Access sheet by zero-based index.
        @param[in] sheetIndex Sheet index (zero-based).
        @return Reference to ODSSheet instance.
    */
    virtual ODSSheet& sheet(ODSSize sheetIndex) = 0;
    /*!
        Append new sheet to ODF spreadsheet file.
        @return Index of the new sheet.
    */ 
    virtual ODSSize appendSheet() = 0;
};


}//namespace

#endif //CPPODSREPORTCORE_H
