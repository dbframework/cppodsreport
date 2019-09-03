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

/*! \file cppodsreport.h
     Include this file in your project to use cppodsreport library.
*/

#include "datasourceabstract.h"
#include "cppodsreportexpdef.h"
#include "cppodsreportcore.h"

extern "C" {
    /*!
        Generate report by substituting variables in the template file with data values from provided DataSource.
        @param[in] fileName Template file name in UTF-8 encoding. 
        @param[in] dataSource Pointer to DataSource instance.
        @param[out] intError Pointer to variable to store internal error (see odserror.h) or nullptr.
        @param[out] zipError Pointer to variable to store libzip error or nullptr.
        @param[out] zlibError Pointer to variable to store zlib error or nullptr.
        @param[out] sysError Pointer to variable to store system error (errno) or nullptr.
        @return True if success.
    */
    CPPODSREPORT_API bool report(const char* fileName, cppodsreport::DataSource* dataSource, int* intError, int* zipError, int* zlibError, int* sysError);

#ifdef CPPODSREPORT_WIN
    /*!
        Generate report by substituting variables in the template file with data values from provided DataSource.
        Available only for Windows platform.
        @param[in] fileName Template file name in ANSI (Windows) encoding.
        @param[in] dataSource Pointer to DataSource instance.
        @param[out] intError Pointer to variable to store internal error (see odserror.h) or nullptr.
        @param[out] zipError Pointer to variable to store libzip error or nullptr.
        @param[out] zlibError Pointer to variable to store zlib error or nullptr.
        @param[out] sysError Pointer to variable to store system error (errno) or nullptr.
        @return True if success.
    */
    CPPODSREPORT_API bool reporta(const char* fileName, cppodsreport::DataSource* dataSource, int* intError, int* zipError, int* zlibError, int* sysError);
    /*!
        Generate report by substituting variables in the template file with data values from provided DataSource.
        Available only for Windows platform.
        @param[in] fileName Template file name in UTF-16 encoding.
        @param[in] dataSource Pointer to DataSource instance.
        @param[out] intError Pointer to variable to store internal error (see odserror.h) or nullptr.
        @param[out] zipError Pointer to variable to store libzip error or nullptr.
        @param[out] zlibError Pointer to variable to store zlib error or nullptr.
        @param[out] sysError Pointer to variable to store system error (errno) or nullptr.
        @return True if success.
    */
    CPPODSREPORT_API bool reportw(const wchar_t* fileName, cppodsreport::DataSource* dataSource, int* intError, int* zipError, int* zlibError, int* sysError);
#endif

    /*!
        Get ODFPackage instance. 
        @return Pointer to ODFPackage instance. Release it after usage.
    */
    CPPODSREPORT_API cppodsreport::ODFPackage* create();
    /*!
        Release ODFPackage instance, obtained bu create(). 
        @param[in] p Pointer to ODFPackage instance. 
    */
    CPPODSREPORT_API void release(cppodsreport::ODFPackage* p);
}

namespace cppodsreport {

/*!
    The ODFPackageWrapper class implements automatic allocation and deallocation of ODFPackage instance.
*/
class ODFPackageWrapper {
private:
    ODFPackage* m_ptr;
public:
    /*!
        Create ODFPackageWrapper, allocate ODFPackage instance.
    */
    ODFPackageWrapper()
    {
        m_ptr = create();
    };
    /*!
        Destroy ODFPackageWrapper, deallocate ODFPackage instance.
    */
    ~ODFPackageWrapper()
    {
        if (m_ptr != nullptr)
            release(m_ptr);
    };
    /*!
        Get pointer to ODFPackage instance.
    */
    ODFPackage* operator()()
    {
        return m_ptr;
    };
};

};

#endif // CPPODSREPORT_H
