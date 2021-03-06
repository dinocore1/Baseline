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

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <baseline/Baseline.h>
#include <baseline/Atomic.h>
#include <baseline/CircleBuffer.h>
#include <baseline/Streams.h>
#include <baseline/SharedBuffer.h>
#include <baseline/Hash.h>
#include <baseline/BaseEncoding.h>

using namespace baseline;

TEST_CASE( "circle buffer put single", "[CircleBuffer]" )
{
  CircleBuffer<int> buffer( 5 );
  REQUIRE( buffer.size() == 0 );
  buffer.put( 1 );
  REQUIRE( buffer.size() == 1 );
  buffer.put( 2 );
  buffer.put( 3 );
  buffer.put( 4 );
  buffer.put( 5 );
  REQUIRE( buffer.size() == 5 );
  REQUIRE( buffer.get() == 1 );
  REQUIRE( buffer.size() == 4 );
  REQUIRE( buffer.get() == 2 );
  buffer.put( 6 );
  REQUIRE( buffer.get() == 3 );
}

TEST_CASE( "circle buffer put multiple", "[CircleBuffer]" )
{
  int myarray[10];
  int num;
  CircleBuffer<int> buffer( 5 );
  int n = buffer.write( myarray, 10 );
  REQUIRE( n <= 5 );
  REQUIRE( buffer.size() == n );
  REQUIRE( buffer.available() == 5 - n );
}

TEST_CASE( "atomic inc", "[Atomic]" )
{
  int32_t value = 0;
  REQUIRE( atomic_inc( &value ) == 0 );
  REQUIRE( value == 1 );
}

TEST_CASE( "atomic dec", "[Atomic]" )
{
  int32_t value = 0;
  REQUIRE( atomic_dec( &value ) == 0 );
  REQUIRE( value == -1 );
}

TEST_CASE( "ByteArrayOutputStream has correct data", "[ByteArrayOutputStream]" )
{
  ByteArrayOutputStream out( 4 );
  uint8_t buf[32];

  buf[0] = 'a';
  buf[1] = 'b';
  buf[2] = 'c';

  REQUIRE( out.size() == 0 );
  REQUIRE( out.write( buf, 0, 3 ) == 3 );
  REQUIRE( out.size() == 3 );

  buf[1] = 'd';
  buf[2] = 'e';
  buf[3] = 'f';

  out.write( buf, 1, 3 );

  const uint8_t* buf2 = ( const uint8_t* )out.toSharedBuffer();
  REQUIRE( buf2[0] == 'a' );
  REQUIRE( buf2[1] == 'b' );
  REQUIRE( buf2[2] == 'c' );
  REQUIRE( buf2[3] == 'd' );
  REQUIRE( buf2[4] == 'e' );
  REQUIRE( buf2[5] == 'f' );

  out.close();

}

TEST_CASE( "ByteArrayInputStream outputs correct data", "[ByteArrayInputStream]" )
{
  SharedBuffer* buf = SharedBuffer::alloc( 10 );
  uint8_t* d = ( uint8_t* ) buf->data();
  d[0] = 'x';
  d[1] = 'y';
  d[2] = 'z';

  uint8_t b2[32];

  ByteArrayInputStream bin( buf, 0, 3 );

  REQUIRE( bin.read( b2, 0, 2 ) == 2 );
  REQUIRE( b2[0] == 'x' );
  REQUIRE( b2[1] == 'y' );


  bin.close();
  buf->release();
}

TEST_CASE( "hex encoding works", "[HexEncoding]" )
{
  uint8_t data[] {
    0xEB, 0x8E, 0xBA, 0x67
  };

  BaseEncoding& hex = hexEncoding();
  String8 str = hex.encode( data, 4 );
  str.toUpper();
  String8 expected( "EB8EBA67" );

  REQUIRE( str == expected );

}

struct IntT : public Comparable<IntT> {
  int compare( const IntT& rhs ) const {
    return mValue - rhs.mValue;
  }

  IntT( int v ) : mValue( v ) {}
  int mValue;
};

/*
TEST_CASE( "trivial compare works", "[Comparable]" )
{
  IntT a( 5 );
  IntT b( 5 );
  IntT c( 8 );

  REQUIRE( a == b );
  REQUIRE( a <= b );
  REQUIRE( a >= b );
  REQUIRE( !( a != b ) );
  REQUIRE( a < c );
  REQUIRE( a <= c );
  REQUIRE( c > a );
  REQUIRE( c >= a );

}
*/

TEST_CASE( "string compare works", "[String8]" )
{
  String8 a( "alice" );
  String8 b( "bob" );

  REQUIRE( a == String8( "alice" ) );
  REQUIRE( a <= a );
  REQUIRE( a >= a );
  REQUIRE( a != b );
  REQUIRE( a < b );
  REQUIRE( b > a );
  REQUIRE( a == "alice" );

}

TEST_CASE( "HashCode compare works", "[HashCode]" )
{
  uint8_t buf[] = {
    0x3a, 0xa8
  };

  HashCode a( buf, 2 );

  REQUIRE( a == HashCode( buf, 2 ) );
  REQUIRE( a != HashCode( buf, 1 ) );
}

TEST_CASE( "crc32 works", "[CRC32]" )
{
  uint8_t buf[] = {
    'x', 'y', 'z'
  };

  up<HashFunction> hash = createCRC32();
  hash->update( buf, 2 );
  hash->update( &buf[2], 1 );
  HashCode value = hash->finalize();
  String8 str = value.toHexString();
  str.toUpper();

  REQUIRE( str == String8( "EB8EBA67" ) );

}

TEST_CASE( "SHA1 works", "[SHA1]" )
{
  uint8_t buf[] = {
    'x', 'y', 'z'
  };

  up<HashFunction> hash = createSHA1();
  hash->update( buf, 3 );
  HashCode value = hash->finalize();
  String8 str = value.toHexString();
  str.toLower();

  REQUIRE( str == String8( "66b27417d37e024c46526c2f6d358a754fc552f3" ) );
}

