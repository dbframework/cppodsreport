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
#include "cziparchive.h"
#include <stdio.h>

using namespace cppzip;

CZipArchive::CZipArchive()
{
    m_zip = nullptr;
}

CZipArchive::~CZipArchive()
{    
    close();
}

bool CZipArchive::open(const char* fileName)
{
    close();
    int error;
    m_zip = zip_open(fileName, 0, &error);
    if (m_zip == nullptr)
        setZipError(error);

    return (m_zip != nullptr);
}

#ifdef CPPODSREPORT_WIN
bool CZipArchive::openFromSource(zip_source_t* src)
{
    close();    
    m_zip = zip_open_from_source(src, 0, zip_source_error(src));
    if (m_zip == nullptr) {
        setZipError(zip_source_error(src));
        zip_source_close(src);
    }
    return (m_zip != nullptr);
}

bool CZipArchive::opena(const char* fileName)
{    
    zip_source_t* src = zip_source_win32a_create(fileName, 0, 0, nullptr);
    if (src != nullptr) {
        return openFromSource(src);
    }
    else {
        setInternalError(ERR_NULL_ZIPSRC);
        return false;
    }
}

bool CZipArchive::open(const wchar_t* fileName)
{    
    zip_source_t* src = zip_source_win32w_create(fileName, 0, 0, nullptr);
    if (src != nullptr) {
        return openFromSource(src);
    }
    else {
        setInternalError(ERR_NULL_ZIPSRC);
        return false;
    }
}
#endif

bool CZipArchive::isOpen() const
{
    return (m_zip != nullptr);
}

bool CZipArchive::close()
{    
    bool result = true;

    if (m_zip) {
        if(0 != zip_close(m_zip)) {
            setZipError(zip_get_error(m_zip));
            result = false;            
        }
        m_zip = nullptr;
    }
    return result;
}

bool CZipArchive::extractFile(const char* fileName, const char* newName)
{
    if (!checkOpened())
        return false;

    bool result = true;

    zip_file_t* source = zip_fopen(m_zip, fileName, 0);
    if (source == nullptr) {
        result = false;
        setZipError(zip_get_error(m_zip));
    }

    FILE* destination = nullptr;
    if (result) {
        destination = fopen(newName, "wb");
        if (destination == nullptr) {
            result = false;
            setInternalError(ERR_WRITE_EXTRACTED);
        }
    }

    if (result) {
        const int bufSize = 4096;
        unsigned char* buf = new unsigned char[bufSize];
        zip_int64_t bytesRead;
        size_t bytesWritten;
        do {
            bytesRead = zip_fread(source, buf, bufSize);
            if (bytesRead > 0) {
                bytesWritten = fwrite(buf, 1, bytesRead, destination);
                if (bytesWritten != bytesRead) {
                    result = false;
                    setInternalError(ERR_WRITE_EXTRACTED);
                }
            }
            else if (bytesRead < 0) {
                result = false;
                setInternalError(ERR_WRITE_EXTRACTED);
            }
        } while ((bytesRead > 0) && result);

        delete[] buf;
    }

    if (source != nullptr)
        zip_fclose(source);

    if (destination != nullptr)
        fclose(destination);

    return result;
}

bool CZipArchive::addFile(const char* fileName, const char* fileInZipName, bool replaceIfExists)
{
    if (!checkOpened())
        return false;

    bool result = true;

    zip_source_t* source = zip_source_file(m_zip, fileName, 0, 0);
    if (source == nullptr) {
        result = false;
        setZipError(zip_get_error(m_zip));
    }

    if (result) {
        result = addFile(source, fileInZipName, replaceIfExists);
    }

    return result;
}

bool CZipArchive::addFile(const void *content, ZipInt size, const char* fileInZipName, bool replaceIfExists)
{
    if (!checkOpened())
        return false;

    bool result = true;

    zip_source_t* source = zip_source_buffer(m_zip, content, size, 0);
    if (source == nullptr) {
        result = false;
        setZipError(zip_get_error(m_zip));
    }

    if (result) {
        result = addFile(source, fileInZipName, replaceIfExists);
    }

    return result;
}

bool CZipArchive::addFile(zip_source_t* src, const char* fileInZipName, bool replaceIfExists)
{
    bool result = true;

    if (src == nullptr) {
        result = false;
        setInternalError(ERR_NULL_ZIPSRC);
    }

    if (result) {
        if (zip_file_add(m_zip, fileInZipName, src, replaceIfExists ? ZIP_FL_OVERWRITE : 0) < 0) {
            result = false;
            zip_source_free(src);
            setZipError(zip_get_error(m_zip));
        }
    }

    return result;
}


ZipArchiveFile* CZipArchive::openFileInZip(ZipInt index)
{
    ZipArchiveFile* result = nullptr;

    if (!checkOpened())
        return result;

    zip_file_t* file = zip_fopen_index(m_zip, index, 0);
    if (file != nullptr) {
        result = new ZipArchiveFile(file);
    }
    else {
        setZipError(zip_get_error(m_zip));
    }

    return result;
}

ZipArchiveFile* CZipArchive::openFileInZip(const char* fileInZipName)
{
    ZipArchiveFile* result = nullptr;

    if (!checkOpened())
        return result;

    ZipInt index = findFileInZip(fileInZipName);
    if (-1 != index)
        result = openFileInZip(index);

    return result;
}

CZipArchive::ZipInt CZipArchive::findFileInZip(const char* fileInZipName)
{
    if (!checkOpened())
        return -1;

    CZipArchive::ZipInt result = zip_name_locate(m_zip, fileInZipName, 0);
    if (-1 == result)
        setZipError(zip_get_error(m_zip));
    return result;
}

bool CZipArchive::fileStat(const char* fileInZipName, ZipFileStat& stat)
{
    bool result = false;

    if (!checkOpened())
        return result;

    ZipInt index = findFileInZip(fileInZipName);
    if (-1 != index) {
        result = fileStat(index, stat);
    }

    return result;
}

bool CZipArchive::fileStat(ZipInt index, ZipFileStat& stat)
{
    bool result = false;

    if (!checkOpened())
        return result;

    if (0 == zip_stat_index(m_zip, index, 0, &stat.m_data))
        result = true;

    return result;
}

void CZipArchive::clearErrors()
{
    m_internalError = 0;
    m_zip_error = m_zlib_error = m_sys_error = 0;
}

void CZipArchive::setInternalError(int error)
{
    clearErrors();
    m_internalError = error;
}

void CZipArchive::setZipError(zip_error* error)
{
    clearErrors();
    m_zip_error = zip_error_code_zip(error);
    int errType = zip_error_system_type(error);
    if (ZIP_ET_ZLIB == errType) {
        m_zlib_error = zip_error_code_system(error);
    }
    else {
        m_sys_error = zip_error_code_system(error);
    }
}

void CZipArchive::setZipError(int error)
{
    clearErrors();
    m_zip_error = error;
}

bool CZipArchive::checkOpened()
{
    bool result = (m_zip != nullptr);
    if (!result)
        setInternalError(ERR_CLOSED);
    return result;
}

int CZipArchive::zipError() const
{
    return m_zip_error;
}

int CZipArchive::zlibError() const
{
    return m_zlib_error;
}

int CZipArchive::sysError() const
{
    return m_sys_error;
}

int CZipArchive::internalError() const
{
    return m_internalError;
}
