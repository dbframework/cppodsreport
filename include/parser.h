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
#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <list>

namespace cppodsreport {

/*!
    The Parser class extracts commands and variables of special format from the text. In the description
    below mandatory elements are enclosed in <>,  optional elements are enclosed in [].

    Command format:
    <symbol cmdBegin><H or V><,><block height or *><,><block width or *><,><datasource name><symbol cmdEnd>
    Command examples (cmdBegin = '{', cmdEnd = '}'):
    {V,2,3,testds} - defines block of 2x3 cells growing vertically assosiated with testds;
    {V,1,*,testds} - defines block of one row of cells growing vertically assosiated with testds;
    {H,*,1,testds} - defines block of one column of cells growing horizontally assosiated with testds.

    Variable format:
    <symbol varBegin><datasource name><.>[nested datasource name]<.>...<.>[nested datasource name]<.><field name><symbol varEnd>
    Variable example (varBegin = '[', varEnd = ']'):
    [MainDatasource.NestedDatasource.Field]
*/
class Parser
{
public:
    typedef std::list<std::wstring> Args;
private:
    Parser();
    Parser(Parser& ps);
    static Parser* m_instance;
    wchar_t m_cmdBegin;
    wchar_t m_cmdEnd;
    wchar_t m_varBegin;
    wchar_t m_varEnd;
    wchar_t m_cmdSep;
    void parseArgs(wchar_t sep, const std::wstring& text, Args& args);
public:
    static Parser* instance();    
    bool parse(const std::wstring& text, std::wstring& left, std::wstring& right,
               Args& cmd, std::wstring& var);
    wchar_t commandBegin() const;
    wchar_t commandEnd() const;
    wchar_t commandSeparator() const;
    wchar_t variableBegin() const;
    wchar_t variableEnd() const;
};

}
#endif // PARSER_H
