#include "cppodsreporttest.h"
#include <iostream>

using namespace std;
using namespace cppodsreport;

TestFile::TestFile(const std::string& path, const std::string& fileName) :
    m_path(path), m_fileName(fileName)
{
    
}

void TestFile::run()
{
    ODFPackageWrapper file;
    if (file() == nullptr) {
        fail("can't create ODFPackage");
        return;
    }
        
    try {
        string fullName = m_path + m_fileName;
#ifdef CPPODSREPORT_WIN
        bool b = file()->opena(fullName.c_str());
#else
        bool b = file()->open(fullName.c_str());
#endif
        if (b) {
            processFile(file());

            b = file()->save();
            if (!b)
                fail("can't save file");
        }
        else {
            fail("can't open file");
        }
    }
    catch (...) {
        fail("exception raised");
    }
    file()->close();
}

void TestFile::fail(const std::string& msg) const
{
    cout << m_fileName << " processing failed: " << msg << endl;
}
