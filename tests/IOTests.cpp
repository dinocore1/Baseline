#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <baseline/Baseline.h>
#include <baseline/Atomic.h>
#include <baseline/CircleBuffer.h>

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