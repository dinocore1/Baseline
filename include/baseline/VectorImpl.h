/*
 * Copyright (C) 2007 The Android Open Source Project
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
 * Modifications by Paul Soucy copyright (C) 2018
 */

#ifndef BASELINE_VECTORIMPL_H_
#define BASELINE_VECTORIMPL_H_

namespace baseline {

class VectorImpl
{
public:
  enum { // flags passed to the ctor
    HAS_TRIVIAL_CTOR    = 0x00000001,
    HAS_TRIVIAL_DTOR    = 0x00000002,
    HAS_TRIVIAL_COPY    = 0x00000004,
  };

  VectorImpl( size_t itemSize, uint32_t flags );
  VectorImpl( const VectorImpl& rhs );
  virtual ~VectorImpl();

  /*! must be called from subclasses destructor */
  void finish_vector();

  VectorImpl& operator = ( const VectorImpl& rhs );
  VectorImpl& operator = ( const VectorImpl& rhs ) const {
    VectorImpl::operator = ( rhs );
    return *const_cast<VectorImpl*>( this );
  }

  /*! C-style array access */
  inline  const void* arrayImpl() const       {
    return mStorage;
  }
  void* editArrayImpl();

  /*! vector stats */
  inline size_t size() const        {
    return mCount;
  }
  inline bool isEmpty() const     {
    return mCount == 0;
  }
  size_t capacity() const;
  int setCapacity( size_t size );

  /*! append/insert another vector or array */
  int insertVectorAt( const VectorImpl& vector, size_t index );
  int appendVector( const VectorImpl& vector );
  int insertArrayAt( const void* array, size_t index, size_t length );
  int appendArray( const void* array, size_t length );

  /*! add/insert/replace items */
  int insertAt( size_t where, size_t numItems = 1 );
  int insertAt( const void* item, size_t where, size_t numItems = 1 );
  void pop();
  void push();
  void push( const void* item );
  int add();
  int add( const void* item );
  int replaceAt( size_t index );
  int replaceAt( const void* item, size_t index );

  /*! remove items */
  int removeItemsAt( size_t index, size_t count = 1 );
  void clear();

  const void* itemLocation( size_t index ) const;
  void* editItemLocation( size_t index );

  typedef int ( *compar_t )( const void* lhs, const void* rhs );
  typedef int ( *compar_r_t )( const void* lhs, const void* rhs, void* state );
  status_t sort( compar_t cmp );
  status_t sort( compar_r_t cmp, void* state );

protected:
  size_t itemSize() const;
  void release_storage();

  virtual void do_construct( void* storage, size_t num ) const = 0;
  virtual void do_destroy( void* storage, size_t num ) const = 0;
  virtual void do_copy( void* dest, const void* from, size_t num ) const = 0;
  virtual void do_splat( void* dest, const void* item, size_t num ) const = 0;
  virtual void do_move_forward( void* dest, const void* from, size_t num ) const = 0;
  virtual void do_move_backward( void* dest, const void* from, size_t num ) const = 0;

  // take care of FBC...
  virtual void reservedVectorImpl1();
  virtual void reservedVectorImpl2();
  virtual void reservedVectorImpl3();
  virtual void reservedVectorImpl4();
  virtual void reservedVectorImpl5();
  virtual void reservedVectorImpl6();
  virtual void reservedVectorImpl7();
  virtual void reservedVectorImpl8();

private:
  void* _grow( size_t where, size_t amount );
  void  _shrink( size_t where, size_t amount );

  inline void _do_construct( void* storage, size_t num ) const;
  inline void _do_destroy( void* storage, size_t num ) const;
  inline void _do_copy( void* dest, const void* from, size_t num ) const;
  inline void _do_splat( void* dest, const void* item, size_t num ) const;
  inline void _do_move_forward( void* dest, const void* from, size_t num ) const;
  inline void _do_move_backward( void* dest, const void* from, size_t num ) const;

  // These 2 fields are exposed in the inlines below,
  // so they're set in stone.
  void* mStorage;   // base address of the vector
  size_t mCount;     // number of items

  const uint32_t mFlags;
  const size_t mItemSize;
};



class SortedVectorImpl : public VectorImpl
{
public:
  SortedVectorImpl( size_t itemSize, uint32_t flags );
  SortedVectorImpl( const VectorImpl& rhs );
  virtual ~SortedVectorImpl();

  SortedVectorImpl& operator = ( const SortedVectorImpl& rhs );

  //! finds the index of an item
  int indexOf( const void* item ) const;

  //! finds where this item should be inserted
  size_t orderOf( const void* item ) const;

  //! add an item in the right place (or replaces it if there is one)
  int add( const void* item );

  //! merges a vector into this one
  int merge( const VectorImpl& vector );
  int merge( const SortedVectorImpl& vector );

  //! removes an item
  int remove( const void* item );

protected:
  virtual int do_compare( const void* lhs, const void* rhs ) const = 0;

  // take care of FBC...
  virtual void            reservedSortedVectorImpl1();
  virtual void            reservedSortedVectorImpl2();
  virtual void            reservedSortedVectorImpl3();
  virtual void            reservedSortedVectorImpl4();
  virtual void            reservedSortedVectorImpl5();
  virtual void            reservedSortedVectorImpl6();
  virtual void            reservedSortedVectorImpl7();
  virtual void            reservedSortedVectorImpl8();

private:
  int _indexOrderOf( const void* item, size_t* order = 0 ) const;

  // these are made private, because they can't be used on a SortedVector
  // (they don't have an implementation either)
  int add();
  void pop();
  void push();
  void push( const void* item );
  int insertVectorAt( const VectorImpl& vector, size_t index );
  int appendVector( const VectorImpl& vector );
  int insertArrayAt( const void* array, size_t index, size_t length );
  int appendArray( const void* array, size_t length );
  int insertAt( size_t where, size_t numItems = 1 );
  int insertAt( const void* item, size_t where, size_t numItems = 1 );
  int replaceAt( size_t index );
  int replaceAt( const void* item, size_t index );
};

}

#endif // BASELINE_VECTORIMPL_H_