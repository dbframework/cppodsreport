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
#ifndef CZIPARCHIVE_H
#define CZIPARCHIVE_H

/*
    Preserve order of following preprocessor directives.
*/
#include "zip.h"
#include "ziparchivefile.h"
#include "zipfilestat.h"

namespace cppzip {

/*
    The CZipArchive is a class used to manipulate zip archives.
 */
class CZipArchive
{
private:
    zip_t* m_zip;
    int m_zip_error;
    int m_zlib_error;
    int m_sys_error;
    int m_internalError;

    CZipArchive(CZipArchive& ){};
    CZipArchive& operator=(const CZipArchive&){return *this;};
public:
    enum Errors {
        ERR_CLOSED = 1,
        ERR_WRITE_EXTRACTED = 2,
        ERR_NULL_ZIPSRC =3
    };
    /*!
        Creates an object.
    */
    CZipArchive();
    /*!
        Destroys the object. If an archive was opened closes it with close().
    */
    ~CZipArchive();
    /*!
        This function opens the archive.
        @param[in] fileName The archve file name in UTF-8 encoding.
        @return True if success.
    */
    bool open(const char* fileName);
#ifdef CPPODSREPORT_WIN
    /*!
        This function opens the archive.
        @param[in] fileName The archve file name in ANSI encoding.
        @return True if success.
    */
    bool opena(const char* fileName);
    /*!
        This function opens the archive.
        @param[in] fileName The archve file name in UTF-8 encoding.
        @return True if success.
    */
    bool open(const wchar_t* fileName);
#endif
    /*!
        This function checks if the archive is already opened.
        @return True if archive is opened.
     */
    bool isOpen() const;
    /*!
        This function saves all changes to archive and closes the archive file.
        @return True if changes were written successfully.
    */
    bool close();
    /*!
        This function extracts file from the archive. The archive must be opened.
        @param[in] fileInZipName Name of the file in the archive.
        @param[in] fileName File name (may be with path) where to extract fileName.
        @return True if success.
     */
    bool extractFile(const char* fileInZipName, const char* fileName);
    /*!
        This function adds file to the archive. The archive must be opened.
        @param[in] fileName Name of the file to add to the  archive.
        @param[in] fileInZipName Name of the file in the archive.
        @param[in] replaceIfExists If true and fileInZipName already exists then it will be replaced.
            Otherwise function fails.
        @return True if success.
     */
    bool addFile(const char* fileName, const char* fileInZipName, bool replaceIfExists);
    /*!
        ZipInt is the shortcut for zip_int64_t type.
     */
    typedef zip_int64_t ZipInt;
    /*!
        This function adds file to the archive. The archive must be opened.
        @param[in] content Pointer to the buffer with file content.
        @param[in] size Size of the buffer in bytes.
        @param[in] fileInZipName Name of the file in the archive.
        @param[in] replaceIfExists If true and fileInZipName already exists then it will be replaced.
            Otherwise function fails.
        @return True if success.
     */
    bool addFile(const void* content, ZipInt size, const char* fileInZipName, bool replaceIfExists);
    /*!
        The findFileInZip method finds file inside the archive.
        @param[in] fileInZipName File name.
        @return If file was found it's index is returned. Returned index can be used by some methods for
        fast file access. If file wasn't found -1 is returned.
     */
    ZipInt findFileInZip(const char* fileInZipName);

    /*!
        The openFileInZip function finds the file inside the archive by name and opens it for reading.
        @param[in] fileInZipName Name of the file inside the archive.
        @return If success, pointer to the allocated instance of ZipArchiveFile is returned. Deallocate
        it with delete after use. If error, nullptr is returned.
     */
    ZipArchiveFile* openFileInZip(const char* fileInZipName);
    /*!
        The openFileInZip function finds the file inside the archive by index and opens it for reading.
        @param[in] fileInZipName Name of the file inside the archive.
        @return If success, pointer to the allocated instance of ZipArchiveFile is returned. Deallocate
        it with delete after use. If error, nullptr is returned.
     */
    ZipArchiveFile* openFileInZip(ZipInt index);
    /*!
        The fileStat function gets information about a file in the archive.
        @param[in] fileInZipName Name of the file in the archive.
        @param[out] stat The function fills this class if success.
        @return True if success.
     */
    bool fileStat(const char* fileInZipName, ZipFileStat& stat);
    /*!
        The fileStat function gets information about a file in the archive.
        @param[in] index Index of the file in the archive.
        @param[out] stat The function fills this class if success.
        @return True if success.
     */
    bool fileStat(ZipInt index, ZipFileStat& stat);
    /*!
        Get libzip error for last operation.
        @return Libzip error.
    */
    int zipError() const;
    /*!
        Get zlib error for last operation.
        @return Zlib error.
    */
    int zlibError() const;
    /*!
        Get errno for last operation.
        @return Errno.
    */
    int sysError() const;
    /*!
         Get internal CZipArchive error for last operation.
         @return Internal CZipArchive error.
    */
    int internalError() const;
private:
    bool addFile(zip_source_t* src, const char* fileInZipName, bool replaceIfExists);
#ifdef CPPODSREPORT_WIN
    bool openFromSource(zip_source_t* src);
#endif
    void clearErrors();    
    bool checkOpened();
    void setInternalError(int error);
    void setZipError(zip_error* error);
    void setZipError(int error);
};

}

#endif // CZIPARCHIVE_H
