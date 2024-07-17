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

#ifndef REPEATEDITEMS_H
#define REPEATEDITEMS_H

#include <list>
#include <stdexcept>

template <typename Item, typename ItemArg>
class RepeatedItems : public std::list<Item>
{
public:
	using size_type = typename std::list<Item>::size_type;
    using iterator = typename std::list<Item>::iterator;
private:
    //ItemArg m_itemArg;

    void appendEmtyRows(size_type count, ItemArg arg)
    {
        this->push_back(Item(arg));
        this->back().setRepeatCount(count);
    };

    Item& extractRow(iterator& repeatedRow, size_type index)
    {
        size_type repeatCount = repeatedRow->repeatCount();

        if (index > repeatCount - 1)
            throw std::out_of_range("RepeatedItems::extractRow - invalid index");

        if (repeatCount == 1)
            return *repeatedRow;

        iterator res;
        Item& r = *repeatedRow;

        if (index == 0) {//need first row 
            r.setRepeatCount(repeatCount - 1);
            res = repeatedRow;
        }
        else {
            r.setRepeatCount(index);
            res = next(repeatedRow);
            if (index != repeatCount - 1) {//row is not last, need to add rows after
                res = this->insert(res, Item(r));
                res->setRepeatCount(repeatCount - index - 1);
            }
        }

        res = this->insert(res, Item(r));
        res->setRepeatCount(1);
        return *res;       
    };

public:
    //RepeatedItems(ItemArg arg) : m_itemArg(arg) {};

    Item& forceItem(size_type row, ItemArg arg)
    {
        size_type r = 0;
        iterator i;
        for (i = this->begin(); i != this->end(); ++i) {
            r += i->repeatCount();
            if (r > row)
                break;
        }

        if (i == this->end()) {//need to add new rows
            size_type rowsToAdd = row - r;
            if (rowsToAdd)
                appendEmtyRows(rowsToAdd, arg);
            appendEmtyRows(1, arg);
            return this->back();
        }
        else if (i->repeatCount() > 1) {//row is repeated row, need to extract one row           
            size_type indexInRow = row - (r - i->repeatCount());
            return extractRow(i, indexInRow);
        }
        else {//row found
            return *i;
        }
    };

/*    RepeatedItems(const RepeatedItems& r) :
        std::list<Item>::list(r), m_itemArg(r.m_itemArg)
    {
        
    };

    RepeatedItems& operator=(const RepeatedItems& rhs)
    {
        std::list<Item>::operator=(rhs);
        m_itemArg = rhs.m_itemArg;
        return *this;
    };*/
};

#endif //REPEATEDITEMS_H
