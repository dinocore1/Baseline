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
#include <baseline/Hash.h>
#include <baseline/SharedBuffer.h>
#include <baseline/Singleton.h>
#include <baseline/BaseEncoding.h>

namespace baseline {

HashCode::HashCode( void* buf, size_t len )
{
  mBuffer = SharedBuffer::alloc( len );
  memcpy( mBuffer->data(), buf, len );
}

HashCode::HashCode( const HashCode& c )
  : mBuffer( c.mBuffer )
{
  mBuffer->acquire();
}

HashCode::~HashCode()
{
  mBuffer->release();
  mBuffer = nullptr;
}

String8 HashCode::toHexString() const
{
    return hexEncoding().encode(mBuffer);
}

bool HashCode::operator== ( const HashCode& c ) const
{
  bool retval = mBuffer->size() == c.mBuffer->size() &&
                memcmp( mBuffer->data(), c.mBuffer->data(), mBuffer->size() ) == 0;
}

HashFunction::~HashFunction()
{}

////////////////// Crc32 //////////////////

struct Crc32Table : public Singleton<Crc32Table> {
  Crc32Table();
  uint32_t gCrc32Table[256];
};

BASELINE_SINGLETON_STATIC_INSTANCE( Crc32Table );

Crc32Table::Crc32Table()
{
  uint32_t POLYNOMIAL = 0x04C11DB7;
  uint32_t remainder;
  uint8_t b = 0;
  do {
    // Start with the data byte
    remainder = b;
    for( uint32_t bit = 8; bit > 0; --bit ) {
      if( remainder & 1 ) {
        remainder = ( remainder >> 1 ) ^ POLYNOMIAL;
      } else {
        remainder = ( remainder >> 1 );
      }
    }
    gCrc32Table[( size_t )b] = remainder;
  } while( 0 != ++b );
}



class Crc32 : public HashFunction
{
public:
  Crc32();

  void update( void* buf, size_t len ) override;
  HashCode finalize() override;

  uint32_t mHash;



};

Crc32::Crc32()
  : mHash( 0xFFFFFFFF )
{
}

void Crc32::update( void* buf, size_t len )
{
  uint32_t* crc32Table = Crc32Table::getInstance().gCrc32Table;

  uint8_t* data = ( uint8_t* ) buf;
  while( len-- ) {
    mHash = ( mHash >> 8 ) ^ crc32Table[( mHash & 0xFF ) ^ *data++];
  }
}

HashCode Crc32::finalize()
{
  mHash = ~mHash;
  return HashCode( &mHash, 4 );
}



HashFunction* crc32()
{
  return new Crc32();
}


}