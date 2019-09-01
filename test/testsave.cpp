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

        ODSSize idx = file()->appendSheet();
        file()->sheet(idx).setSheetName(L"testSheet1");
        file()->sheet(idx).cell(0, 0).setText(L"Next 10 rows will be filled with numbers from 1 to 10");
        for(int i = 1; i < 11; ++i)
            for (int j = 0; j < 10; ++j )
                file()->sheet(idx).cell(i, j).setText(to_wstring(j+1).c_str());

        idx = file()->appendSheet();
        file()->sheet(idx).setSheetName(L"testSheet2");
        file()->sheet(idx).cell(0, 0).setText(L"Next 10 rows will form upper-right triangular matrix with numbers from row number to 10");
        for (int i = 1; i < 11; ++i)
            for (int j = i - 1; j < 10; ++j)
                file()->sheet(idx).cell(i, j).setText(to_wstring(j + 1).c_str());

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