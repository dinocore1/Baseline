#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <baseline/Baseline.h>
#include <baseline/TypeHelpers.h>

using namespace baseline;

TEST_CASE( "struct is pointer", "[TypeHelpers]" )
{

  struct A {};

  REQUIRE( trait_pointer<A>::value == false );
  REQUIRE( trait_pointer<A*>::value == true );
  REQUIRE( trait_pointer<A**>::value == true );

}

TEST_CASE( "trivial triats", "[TypeHelpers]" )
{

  struct A {};

  REQUIRE( traits<A*>::is_pointer == true );
  REQUIRE( traits<A*>::has_trivial_ctor == true );
}
