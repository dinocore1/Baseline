#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <baseline/Baseline.h>
#include <baseline/StrongPointer.h>
#include <baseline/RefBase.h>
#include <baseline/UniquePointer.h>

using namespace baseline;


TEST_CASE( "strong pointer", "[StrongPointer]" )
{
#define DESTRUCTOR  0x01
#define USE         0x02

  struct MyStruct : public RefBase {
    MyStruct( uint8_t& status )
      : mStatus( status )
    {}

    ~MyStruct() {
      mStatus |= DESTRUCTOR;
    }

    void use() {
      mStatus |= USE;
    }

    uint8_t& mStatus;
  };

  uint8_t status = 0;

  {
    sp<MyStruct> p1( new MyStruct( status ) );
    p1->use();
  }

  REQUIRE( status == ( DESTRUCTOR | USE ) );


}

TEST_CASE( "does free single obj?", "[UniquePointer]" )
{
  static int count = 0;

  struct MyObj {
    MyObj( ) {
      count++;
    }

    ~MyObj() {
      count--;
    }
  };

  {
    up<MyObj> myptr( new MyObj );
    REQUIRE( count == 1 );
  }

  REQUIRE( count == 0 );
}

TEST_CASE( "does release work?", "[UniquePointer]" )
{
  static int count = 0;

  struct MyObj {
    MyObj( ) {
      count++;
    }

    ~MyObj() {
      count--;
    }
  };

  MyObj* rawC;
  {
    up<MyObj> c( new MyObj );
    assert( count == 1 );
    rawC = c.release();
  }
  assert( count == 1 );
  delete rawC;
}

TEST_CASE( "does reset work?", "[UniquePointer]" )
{
  static int count = 0;

  struct MyObj {
    MyObj() {
      count++;
    }

    ~MyObj() {
      count--;
    }
  };

  {
    up<MyObj> c( new MyObj );
    REQUIRE( count == 1 );
    c.reset( new MyObj );
    REQUIRE( count == 1 );
  }
  REQUIRE( count == 0 );
}

TEST_CASE( "does free array?", "[UniquePointer]" )
{
  static int count = 0;

  struct MyObj {
    MyObj( ) {
      count++;
    }

    ~MyObj() {
      count--;
    }
  };


  {
    up<MyObj[]> myptr( new MyObj[4] );
    REQUIRE( count == 4 );
  }

  REQUIRE( count == 0 );
}

TEST_CASE( "does release array work?", "[UniquePointer]" )
{
  static int count = 0;

  struct MyObj {
    MyObj( ) {
      count++;
    }

    ~MyObj() {
      count--;
    }
  };


  MyObj* raw;
  {
    up<MyObj[]> myptr( new MyObj[4] );
    REQUIRE( count == 4 );
    raw = myptr.release();

  }

  REQUIRE( count == 4 );
  delete[] raw;
}

TEST_CASE( "does reset array work?", "[UniquePointer]" )
{
  static int count = 0;

  struct MyObj {
    MyObj() {
      count++;
    }

    ~MyObj() {
      count--;
    }
  };

  {
    up<MyObj[]> c( new MyObj[4] );
    REQUIRE( count == 4 );
    c.reset( new MyObj[2] );
    REQUIRE( count == 2 );
  }

  REQUIRE( count == 0 );
}

TEST_CASE( "does custom deleter work?", "[UniquePointer]" )
{

  static bool freed = false;

  struct Freer {
    void operator()( int* p ) {
      REQUIRE( *p == 123 );
      free( p );
      freed = true;
    }
  };

  {
    up<int, Freer> i( reinterpret_cast<int*>( malloc( sizeof( int ) ) ) );
    *i = 123;
  }

  REQUIRE( freed == true );
}

