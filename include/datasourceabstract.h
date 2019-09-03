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
#ifndef DATASOURCEABSTRACT_H
#define DATASOURCEABSTRACT_H

namespace cppodsreport {

/*!
     The DataSource class is the asbstract interface for implementing report data sources.
     A data source must represent data as a set of tables. Tables are identified by name.
     A table has rows and columns. Table columns are identified by name. One of the table's
     rows is current row. Table cells are accessed by changing the current row and providing
     column name. 
*/
class DataSource {
public:    
    /*!
        This method must implement iteration through the all table rows. If the table has one more row, 
        the method must make this row current and return true, otherwise return false. Making the row
        to be current means that subsequent calls to method value(...) for this table must fetch column 
        value from this row. 

        This method is called only for tables associated with a block. 

        @param[in] tableName Name of the table. The last argument of a block command in the template is passed
        as this parameter.
        @return True if one more row exists in the table, false otherwise.
     */    
    virtual bool next(const wchar_t* tableName) = 0;
    /*!
        This method must look for a table cell identified by columnName parameter. The columnName is the name of variable 
        in the template. It must include table name and column name. 

        If the method haven't found the column identified by columnName it must return false. Otherwise the method must copy table
        cell value to columnValue and return true. If cellValue is too small to fit cell value the method must set size to the 
        required size and return false.

        @param[in] columnName Variable name (an indentificator of a table and a column).
        @param[in] cellValue Allocated buffer where the method must copy cell value (including terminating zero).
        @param[in, out] size Upon call size must contain cellValue size in wchar_t symbols.
        If columnn with columnName is not found the method must not modify size. If cellValue is to small
        the method must set size to the required size in wchar_t symbols. Otherwise the method must set size
        to be actual size of the cell value, copied to cellValue (including terminating zero).
        @return True if success.
     */    
    virtual bool value(const wchar_t* columnName, wchar_t* cellValue, int* size) = 0;
};

}

#endif // DATASOURCEABSTRACT_H
