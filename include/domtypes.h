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
#ifndef DOMTYPES_H
#define DOMTYPES_H

#include <QDomDocument>
#include <QDomNodeList>

namespace cppodsreport {

#define NODE_TYPE_ELEMENT 1
#define NODE_TYPE_TEXT 3

typedef QString DomString;
typedef QDomNode DomNode;
typedef QDomNodeList DomNodeList;

class DomDocument : public QDomDocument
{
public:
    DomDocument() :QDomDocument() {};
    DomDocument(const QDomDocument & x) :QDomDocument(x) {};
    DomDocument& operator=(const QDomDocument& x)
    {
        QDomDocument::operator =(x);
        return *this;
    }
    DomNodeList getElementsByTagNameNS(const DomString & nsURI, const DomString & localName)
    {
        return elementsByTagNameNS(nsURI, localName);
    }
};

class DomElement : public QDomElement
{
public:
    DomElement():QDomElement(){};
    DomElement(const QDomElement & x):QDomElement(x){};
    DomElement& operator=(const QDomElement& x)
    {
        QDomElement::operator =(x);
        return *this;
    }
    DomString  getAttributeNS(DomString namespaceURI, DomString localName)
    {
        return attributeNS(namespaceURI, localName);
    }
    operator bool()
    {
        return !isNull();
    }
};


}

#endif // DOMTYPES_H
