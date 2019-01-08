/*
 * Copyright (C) 2007 Baseline
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef BASELINE_LINKEDLIST_H_
#define BASELINE_LINKEDLIST_H_

namespace baseline {

template<typename T>
class LinkedList
{
public:
    LinkedList();
    ~LinkedList();

    inline
    size_t size() const;

    inline
    bool empty() const;

    inline
    void clear();

    inline
    T& head();
    
    inline
    const T& head() const;

    inline
    T& tail();

    inline
    const T& tail() const;

    inline
    void push();

    inline
    void pop();

    inline
    void add(const T& item);

private:
    struct Item {
        Item(const T& item);
        Item* next;
        Item* prev;
        T data;
    };

    uint32_t mSize;
    Item* mHead;
    Item* mTail;

};


/////////////////// Implementation ////////////////////////

template<typename T>
LinkedList<T>::Item::Item(const T& item)
 : next(nullptr), prev(nullptr), data(item)
{}

template<typename T>
LinkedList<T>::LinkedList()
 : mSize(0), mHead(nullptr), mTail(nullptr)
{}

template<typename T>
LinkedList<T>::~LinkedList()
{
    //TODO: delete everything
}

template<typename T>
size_t LinkedList<T>::size() const
{
    return mSize;
}

template<typename T>
bool LinkedList<T>::empty() const
{
    return mSize == 0;
}

template<typename T>
void LinkedList<T>::add(const T& item)
{
    Item* newItem = new Item(item);
    newItem->next = mTail;
    if(empty()) {
        mHead = newItem;
    } else {
        mTail->prev = newItem;
    }
    mTail = newItem;
    mSize++;
}


} // namespace baseline

#endif // BASELINE_LINKEDLIST_H_