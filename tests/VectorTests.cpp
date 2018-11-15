#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <baseline/Baseline.h>
#include <baseline/Vector.h>

using namespace baseline;


TEST_CASE( "copy on write", "[Vector]" )
{
  Vector<int> vector;
  Vector<int> other;
  vector.setCapacity( 8 );

  vector.add( 1 );
  vector.add( 2 );
  vector.add( 3 );

  REQUIRE( vector.size() == 3 );

  // copy the vector
  other = vector;

  REQUIRE( other.size() == 3 );

  // add an element to the first vector
  vector.add( 4 );

  // make sure the sizes are correct
  REQUIRE( vector.size() == 4 );
  REQUIRE( other.size() == 3 );

  // add an element to the copy
  other.add( 5 );

  // make sure the sizes are correct
  REQUIRE( vector.size() == 4 );
  REQUIRE( other.size() == 4 );

  // make sure the content of both vectors are correct
  REQUIRE( vector[3] == 4 );
  REQUIRE( other[3] == 5 );
}