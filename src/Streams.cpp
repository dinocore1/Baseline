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

#include <baseline/Baseline.h>
#include <baseline/Streams.h>
#include <baseline/SharedBuffer.h>

namespace baseline {

void NullOutputStream::close()
{
}

int NullOutputStream::write( uint8_t*, size_t, size_t len )
{
  return len;
}

/////////////// ByteArrayOutputStream //////////////////////

ByteArrayOutputStream::ByteArrayOutputStream( size_t size )
{
  mSize = 0;
  mBuffer = SharedBuffer::alloc( size );
}

ByteArrayOutputStream::~ByteArrayOutputStream()
{
  if( mBuffer != nullptr ) {
    LOG_WARN( "ByteArrayOutputStream", "destory without calling close first" );
    close();
  }
}

size_t ByteArrayOutputStream::size() const
{
  return mSize;
}

void* ByteArrayOutputStream::toSharedBuffer()
{
  return mBuffer->data();
}

void ByteArrayOutputStream::close()
{
  mBuffer->release();
  mBuffer = nullptr;
}

int ByteArrayOutputStream::write( uint8_t* buf, size_t off, size_t len )
{
  if( mSize + len < mBuffer->size() ) {
    mBuffer = mBuffer->edit();
  } else {
    mBuffer = mBuffer->editResize( mSize + len );
  }

  uint8_t* dest = reinterpret_cast<uint8_t*>( mBuffer->data() );
  memcpy( &dest[mSize], &buf[off], len );
  mSize += len;
  return len;
}

/////////////// ByteArrayInputStream //////////////////////

ByteArrayInputStream::ByteArrayInputStream( SharedBuffer* buf, size_t offset, size_t len )
  : mBuffer( buf ), mOffset( 0 ), mLen( len )
{
  mBuffer->acquire();
}

ByteArrayInputStream::~ByteArrayInputStream()
{
  if( mBuffer != nullptr ) {
    LOG_WARN( "ByteArrayInputStream", "destory without calling close first" );
    close();
  }
}

void ByteArrayInputStream::close()
{
  mBuffer->release();
  mBuffer = nullptr;
}

int ByteArrayInputStream::read( uint8_t* buf, size_t off, size_t len )
{

  if( mOffset < mLen ) {
    uint8_t* src = reinterpret_cast<uint8_t*>( mBuffer->data() );
    len = MIN( len, mLen - mOffset );
    memcpy( &buf[off], &src[mOffset], len );
    mOffset += len;
    return len;
  } else {
    return -1;
  }
}

#define BUF_SIZE 1024

void pump( InputStream& in, OutputStream& out, IOProgress* callback, bool closeOutput, bool closeInput )
{
  uint8_t buf[BUF_SIZE];
  int bytesRead;

  while( ( bytesRead = in.read( buf, 0, BUF_SIZE ) ) > 0 ) {
    out.write( buf, 0, bytesRead );
    if( callback != nullptr ) {
      callback->onProgress( bytesRead );
    }
  }

  if( closeInput ) {
    in.close();
  }

  if( closeOutput ) {
    out.close();
  }

}

} // namespace