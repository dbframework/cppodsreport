#include "testsave.h"
#include "cppodsreporttest.h"
#include <iostream>

using namespace cppodsreport;
using namespace std;

TestSave::TestSave(const std::string& path) :
    TestNewFile(path, "testsave.ods")
{

}

void TestSave::processFile(ODFPackage* file)
{
    ODSSize idx = file->appendSheet();
    file->sheet(idx).setSheetName(L"testSheet1");
    file->sheet(idx).cell(0, 0).setText(L"Next 10 rows will be filled with numbers from 1 to 10");
    for (int i = 1; i < 11; ++i)
        for (int j = 0; j < 10; ++j)
            file->sheet(idx).cell(i, j).setText(to_wstring(j + 1).c_str());

    idx = file->appendSheet();
    file->sheet(idx).setSheetName(L"testSheet2");
    file->sheet(idx).cell(0, 0).setText(L"Next 10 rows will form upper-right triangular matrix with numbers from row number to 10");
    for (int i = 1; i < 11; ++i)
        for (int j = i - 1; j < 10; ++j)
            file->sheet(idx).cell(i, j).setText(to_wstring(j + 1).c_str());
}

