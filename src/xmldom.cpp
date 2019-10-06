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
#include "xmldom.h"
#include <memory>

using namespace cppodsreport;
using namespace std;

bool DOMDocumentWrapper::setContent(void* buf, int size)
{
    return m_doc.setContent(QByteArray::fromRawData(static_cast<char*>(buf), size), true);
}

int64_t DOMDocumentWrapper::contentSize() const
{
    return m_doc.toByteArray().size();
}

int DOMDocumentWrapper::getContent(void* buf, int maxSize)
{    
    int result = (maxSize > m_doc.toByteArray().size()) ? m_doc.toByteArray().size() : maxSize;
    memcpy(buf, m_doc.toByteArray().data(), result);
    return result;
}

void DOMDocumentWrapper::clearContent()
{    
    m_doc.clear();
}

DomElement DOMDocumentWrapper::toElement(const DomNode& node)
{
    DomElement result;
    if (node.isElement())
        result = node.toElement();
    return result;
}

std::wstring DOMDocumentWrapper::DomStringToWstring(const DomString& str)
{
	unique_ptr<wchar_t[]> buf(new wchar_t[str.size() + 1]);
	str.toWCharArray(buf.get());
	wchar_t* nullpos = buf.get() + str.size();
	*nullpos = 0;
    return wstring(buf.get());
}

DomString DOMDocumentWrapper::WstringToDomString(const std::wstring& str)
{
    return QString::fromWCharArray(str.c_str());
}

DomString DOMDocumentWrapper::qualifiedName(const char* namespaceTag, const char* name)
{
    return DomString(namespaceTag) + DomString(":") + DomString(name);
}
