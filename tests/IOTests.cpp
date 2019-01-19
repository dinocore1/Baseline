#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <baseline/Baseline.h>
#include <baseline/Atomic.h>
#include <baseline/CircleBuffer.h>
#include <baseline/Streams.h>
#include <baseline/SharedBuffer.h>

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

}