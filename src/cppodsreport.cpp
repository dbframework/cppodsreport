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
#include "cppodsreport.h"
#include "odsfile.h"

using namespace cppodsreport;

bool makeReport(ODSFile& file, DataSource* ds)
{
    bool result;
    result = file.process(ds);
    if (result)
        result = file.save();
    if (result)
        result = file.close();

    return result;
}

void lastError(const ODSFile& file, int* intError, int* zipError, int* zlibError, int* sysError)
{
    ODSFile::ErrorStruct err;
    file.getLastError(err);
    if (intError != nullptr) {
        *intError = err.internalError2;
        if (0 == *intError)
            *intError = err.internalError;
    }
    if (zipError != nullptr)
        *zipError = err.zipError;
    if (zlibError != nullptr)
        *zlibError = err.zlibError;
    if (sysError != nullptr)
        *sysError = err.sysError;
}

bool report(const char* fileName, cppodsreport::DataSource* dataSource, int *intError, int *zipError, int *zlibError, int *sysError)
{
    bool result;
    ODSFile file;
    result = file.open(fileName);
    if (result)
        result = makeReport(file, dataSource);
    if (!result)
        lastError(file, intError, zipError, zlibError, sysError);
    return result;
}

#ifdef CPPODSREPORT_WIN
bool reporta(const char* fileName, cppodsreport::DataSource* dataSource, int *intError, int *zipError, int *zlibError, int *sysError)
{
    bool result;
    ODSFile file;
    result = file.opena(fileName);
    if (result)
        result = makeReport(file, dataSource);
    if (!result)
        lastError(file, intError, zipError, zlibError, sysError);
    return result;
}

bool reportw(const wchar_t* fileName, cppodsreport::DataSource* dataSource, int *intError, int *zipError, int *zlibError, int *sysError)
{
    bool result;
    ODSFile file;
    result = file.open(fileName);
    if (result)
        result = makeReport(file, dataSource);
    if (!result)
        lastError(file, intError, zipError, zlibError, sysError);
    return result;
}
#endif
