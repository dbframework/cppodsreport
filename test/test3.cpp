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

#include "test3.h"

using namespace cppodsreport;

Test3::Test3(const std::string& path) :
    TestFile(path, "test3.ods")
{

}

void Test3::processFile(ODFPackage* file)
{
    file->sheet(0).cell(0, 0).setText(L"A1");
    file->sheet(0).cell(1, 2).setText(L"C2");
    file->sheet(0).cell(2, 1).setText(L"B3");
    file->sheet(0).cell(3, 2).setText(L"C4");
    file->sheet(0).cell(4, 5).setText(L"F5");
    file->sheet(0).cell(6, 0).setText(L"A7");
    file->sheet(0).cell(12, 1).setText(L"B13");
    file->sheet(0).cell(18, 2).setText(L"C19");
    file->sheet(0).cell(21, 6).setText(L"G22");
}