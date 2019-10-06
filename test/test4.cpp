/*
Copyright (c) 2019 Sidorov Dmitry

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

#include "test4.h"

using namespace cppodsreport;

Test4::Test4(const std::string& path) : TestNewFile(path, "test4.ods")
{

}

void Test4::processFile(ODFPackage* file)
{
    ODSSize idx = file->appendSheet();
    ODSSheet&  sheet = file->sheet(idx);
    sheet.setSheetName(L"test");
    
    sheet.cell(0, 0).setText(L"Boolean value");
    sheet.cell(1, 0).setBool(true);

    sheet.cell(0, 1).setText(L"Floating point value");
    sheet.cell(1, 1).setFloat(-1.23e1);

    sheet.cell(0, 2).setText(L"Percentage value");
    sheet.cell(1, 2).setPercentage(1.5);

    sheet.cell(0, 3).setText(L"Currency value");
    sheet.cell(1, 3).setCurrency(10.5, L"USD");

    sheet.cell(0, 4).setText(L"Date value");
    sheet.cell(1, 4).setDate(1970, 2, 5);

    sheet.cell(0, 5).setText(L"Date value");
    sheet.cell(1, 5).setDate(1970, 2, 5, 5, 2, 14.567);

    sheet.cell(0, 6).setText(L"Time value");
    sheet.cell(1, 6).setTime(10, 2, 22.5);
}