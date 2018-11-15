#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <baseline/Baseline.h>
#include <baseline/RefBase.h>

using namespace baseline;


TEST_CASE( "strong pointer", "[StrongPointer]" )
{
#define DESTRUCTOR  0x01
#define USE         0x02

  struct MyStruct : public RefBase {
    MyStruct(uint8_t& status)
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
    sp<MyStruct> p1( new MyStruct(status) );
    p1->use();
  }

  REQUIRE(status == (DESTRUCTOR | USE));


}