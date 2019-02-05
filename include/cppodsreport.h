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
#ifndef CPPODSREPORT_H
#define CPPODSREPORT_H

#include "datasourceabstract.h"
#include "cppodsreportexpdef.h"
#include "cppodsreportcore.h"

extern "C" {
    CPPODSREPORT_API bool report(const char* fileName, cppodsreport::DataSource* dataSource, int* intError, int* zipError, int* zlibError, int* sysError);
#ifdef CPPODSREPORT_WIN
    CPPODSREPORT_API bool reporta(const char* fileName, cppodsreport::DataSource* dataSource, int* intError, int* zipError, int* zlibError, int* sysError);
    CPPODSREPORT_API bool reportw(const wchar_t* fileName, cppodsreport::DataSource* dataSource, int* intError, int* zipError, int* zlibError, int* sysError);
#endif

    CPPODSREPORT_API cppodsreport::ODFPackage* create();
    CPPODSREPORT_API void release(cppodsreport::ODFPackage* p);
}

namespace cppodsreport {

class ODFPackageWrapper {
private:
    ODFPackage* m_ptr;
public:
    ODFPackageWrapper()
    {
        m_ptr = create();
    };
    ~ODFPackageWrapper()
    {
        if (m_ptr != nullptr)
            release(m_ptr);
    };
    ODFPackage* operator()()
    {
        return m_ptr;
    };
};

};

#endif // CPPODSREPORT_H
