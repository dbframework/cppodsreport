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
#include "parser.h"

using namespace cppodsreport;
using namespace std;

Parser* Parser::m_instance = nullptr;

Parser::Parser()
{
    m_cmdBegin = '{';
    m_cmdEnd = '}';
    m_cmdSep = ',';
    m_varBegin = '[';
    m_varEnd = ']';
}

Parser::Parser(Parser& )
{

}

Parser* Parser::instance()
{
    if (m_instance == nullptr)
        m_instance = new Parser;
    return m_instance;
}

bool Parser::parse(const std::wstring& text, std::wstring& left, std::wstring& right,
           Args &cmd, std::wstring &var)
{
    bool result = true;
    bool isCmd = false;
    bool isVar = false;

    wstring::size_type begin;
    wstring::size_type end;
    for (begin = 0; begin < text.size(); ++begin) {
        if (text[begin] == m_cmdBegin) {
            isCmd = true;
            break;
        }
        else if (text[begin] == m_varBegin) {
            isVar = true;
            break;
        }
    }

    if (!isCmd && !isVar) {
        left = text;
        right.clear();
    }
    else {
        if (isCmd) {
            end = text.find(m_cmdEnd, begin + 1);
        }
        else {
            end = text.find(m_varEnd, begin + 1);
        }

        if (end == wstring::npos) {
            result = false;
        }
        else {
            left = text.substr(0, begin);
            right = text.substr(end + 1);
            if (isCmd) {
                parseArgs(m_cmdSep, text.substr(begin, end - begin +1), cmd);
            }
            else {
                var = text.substr(begin + 1, end - begin - 1);
            }
        }
    }


    return result;
}

void Parser::parseArgs(wchar_t sep, const wstring &text, Args& args)
{
    wstring::size_type begin = 1;
    wstring::size_type end = 0;
    for (;; ) {
        end = text.find(sep, begin);
        if (wstring::npos == end) {
            args.push_back(text.substr(begin, text.size() - begin - 1));
            break;
        }
        else {
            args.push_back(text.substr(begin, end - begin));
            begin = end + 1;
        }
    }
}

wchar_t Parser::commandBegin() const
{
    return m_cmdBegin;
}

wchar_t Parser::commandEnd() const
{
    return m_cmdEnd;
}

wchar_t Parser::commandSeparator() const
{
    return m_cmdSep;
}

wchar_t Parser::variableBegin() const
{
    return m_varBegin;
}

wchar_t Parser::variableEnd() const
{
    return m_varEnd;
}
