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
#include <baseline/Vector.h>
#include <baseline/SortedVector.h>

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

TEST_CASE( "trivial sorted vector", "[SortedVector]" )
{
  SortedVector<int> vector;
  vector.add( 2 );
  vector.add( 1 );

  REQUIRE( vector.size() == 2 );
  REQUIRE( vector[0] == 1 );
  REQUIRE( vector[1] == 2 );
}

TEST_CASE( "non-trivial sorted vector", "[SortedVector]" )
{
  struct MyStruct {
    char mKey;
    float mValue;

    bool operator< ( const MyStruct& rhs ) const {
      return mKey < rhs.mKey;
    }

  };

  SortedVector<MyStruct> vector;
  vector.add( {4, 3.14f} );
  vector.add( {2, 11.2f} );

  REQUIRE( vector.size() == 2 );
  REQUIRE( ( vector[0].mKey == 2 && vector[0].mValue == 11.2f ) );
}

TEST_CASE( "vector of pointers", "[Vector]" )
{
  struct MyStruct {
    char mWhateve;

    virtual void* test() = 0;
  };

  struct MyStruct2 : public MyStruct {
    void* test() {
      return NULL;
    }
  };

  Vector<MyStruct*> vector;
  //vector.add( new MyStruct2 );
}