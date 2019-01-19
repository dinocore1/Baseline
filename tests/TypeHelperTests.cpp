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
