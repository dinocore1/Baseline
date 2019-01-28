/*
 * Copyright (C) 2018 Baseline
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

#ifndef BASELINE_CIRCLEBUFFER_H_
#define BASELINE_CIRCLEBUFFER_H_

namespace baseline {

template<typename T>
class CircleBuffer
{
public:
  CircleBuffer( uint32_t capacity );
  ~CircleBuffer();

  inline
  uint32_t capacity() const;

  inline
  uint32_t size() const;

  inline
  uint32_t available() const;

  inline
  bool empty() const;

  inline
  bool full() const;

  inline
  void clear();

  uint32_t write( const T* buf, uint32_t size );
  void put( const T& v );

  uint32_t read( T* buf, uint32_t size );
  T get();

private:
  uint32_t mCapacity;
  uint32_t mSize;
  uint32_t mHead;
  uint32_t mTail;
  T* mData;

};

/////////////// Implementation ////////////////////

template<typename T>
CircleBuffer<T>::CircleBuffer( uint32_t capacity )
  : mCapacity( capacity ), mSize( 0 ), mHead( 0 ), mTail( 0 )
{
  mData = new T[mCapacity];
}

template<typename T>
CircleBuffer<T>::~CircleBuffer()
{
  delete[] mData;
  mCapacity = 0;
  mSize = 0;
}

template<typename T>
uint32_t CircleBuffer<T>::capacity() const
{
  return mCapacity;
}

template<typename T>
uint32_t CircleBuffer<T>::size() const
{
  return mSize;
}

template<typename T>
uint32_t CircleBuffer<T>::available() const
{
  return mCapacity - mSize;
}

template<typename T>
bool CircleBuffer<T>::empty() const
{
  return mSize == 0;
}

template<typename T>
bool CircleBuffer<T>::full() const
{
  return mSize == mCapacity;
}

template<typename T>
void CircleBuffer<T>::clear()
{
  mSize = 0;
  mHead = 0;
  mTail = 0;
}

template <typename T>
uint32_t CircleBuffer<T>::write( const T* buf, uint32_t size )
{
  uint32_t len = MIN( size, mCapacity - mSize );
  len = MIN( len, mCapacity - mHead );
  for( uint32_t i = 0; i < len; i++ ) {
    mData[mHead + i] = buf[i];
  }

  mHead = ( mHead + len ) % mCapacity;
  mSize += len;
  return len;
}

template <typename T>
uint32_t CircleBuffer<T>::read( T* buf, uint32_t size )
{
  uint32_t len = MIN( size, mSize );
  len = MIN( len, mCapacity - mTail );
  for( uint32_t i = 0; i < len; i++ ) {
    buf[i] = mData[mTail + i];
  }

  mTail = ( mTail + len ) % mCapacity;
  mSize -= len;
  return len;
}

template<typename T>
void CircleBuffer<T>::put( const T& v )
{
  write( &v, 1 );
}

template<typename T>
T CircleBuffer<T>::get()
{
  T retval;
  read( &retval, 1 );

  return retval;
}

}

#endif // BASELINE_CIRCLEBUFFER_H_