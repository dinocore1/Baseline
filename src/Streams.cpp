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

#include <baseline/Baseline.h>
#include <baseline/Streams.h>
#include <baseline/SharedBuffer.h>

namespace baseline {


ByteArrayOutputStream::ByteArrayOutputStream( size_t size )
{
  mSize = 0;
  mBuffer = SharedBuffer::alloc( size );
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

  memcpy( &( ( uint8_t* )mBuffer->data() )[mSize], &buf[off], len );
  mSize += len;
  return len;
}

} // namespace