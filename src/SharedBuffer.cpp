
#include <baseline/Baseline.h>
#include <baseline/Atomic.h>
#include <baseline/SharedBuffer.h>

namespace baseline {

SharedBuffer* SharedBuffer::alloc( size_t size )
{
  SharedBuffer* sb = static_cast<SharedBuffer*>( malloc( sizeof( SharedBuffer ) + size ) );
  if( sb ) {
    sb->mRefs = 1;
    sb->mSize = size;
  }
  return sb;
}


int SharedBuffer::dealloc( const SharedBuffer* released )
{
  if( released->mRefs != 0 ) {
    return -1;  // XXX: invalid operation
  }
  free( const_cast<SharedBuffer*>( released ) );
  return 0;
}

SharedBuffer* SharedBuffer::edit() const
{
  if( onlyOwner() ) {
    return const_cast<SharedBuffer*>( this );
  }
  SharedBuffer* sb = alloc( mSize );
  if( sb ) {
    memcpy( sb->data(), data(), size() );
    release();
  }
  return sb;
}

SharedBuffer* SharedBuffer::editResize( size_t newSize ) const
{
  if( onlyOwner() ) {
    SharedBuffer* buf = const_cast<SharedBuffer*>( this );
    if( buf->mSize == newSize ) {
      return buf;
    }
    buf = ( SharedBuffer* )realloc( buf, sizeof( SharedBuffer ) + newSize );
    if( buf != NULL ) {
      buf->mSize = newSize;
      return buf;
    }
  }
  SharedBuffer* sb = alloc( newSize );
  if( sb ) {
    const size_t mySize = mSize;
    memcpy( sb->data(), data(), newSize < mySize ? newSize : mySize );
    release();
  }
  return sb;
}

SharedBuffer* SharedBuffer::attemptEdit() const
{
  if( onlyOwner() ) {
    return const_cast<SharedBuffer*>( this );
  }
  return 0;
}

SharedBuffer* SharedBuffer::reset( size_t new_size ) const
{
  // cheap-o-reset.
  SharedBuffer* sb = alloc( new_size );
  if( sb ) {
    release();
  }
  return sb;
}

void SharedBuffer::acquire() const
{
  atomic_inc( &mRefs );
}

int32_t SharedBuffer::release( uint32_t flags ) const
{
  int32_t prev = 1;
  if( onlyOwner() || ( ( prev = atomic_dec( &mRefs ) ) == 1 ) ) {
    mRefs = 0;
    if( ( flags & eKeepStorage ) == 0 ) {
      free( const_cast<SharedBuffer*>( this ) );
    }
  }
  return prev;
}


}