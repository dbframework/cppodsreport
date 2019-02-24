#include "testsave.h"
#include "cppodsreporttest.h"
#include <iostream>

using namespace cppodsreport;
using namespace std;

TestSave::TestSave(const std::string& path) :
    m_path(path)
{

}

void TestSave::fail(const std::string& msg) const
{
    cout << "testsave.ods processing failed: " << msg << endl;
}


void TestSave::run()
{
    ODFPackageWrapper file;
    if (file() == nullptr) {
        fail("can't create ODFPackage");
        return;
    }

    try {
        string fullName = m_path + "testsave.ods";
#ifdef CPPODSREPORT_WIN
        bool b = file()->savea(fullName.c_str());
#else
        bool b = file()->save(fullName.c_str());
#endif
        if (!b) 
            fail("can't save file");        
    }
    catch (...) {
        fail("exception raised");
    }
    file()->close();
}