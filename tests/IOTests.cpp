#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <baseline/CircleBuffer.h>

using namespace baseline;

TEST_CASE("circle buffer", "[CircleBuffer]") {
  CircleBuffer<int> buffer(5);
  REQUIRE(buffer.size() == 0);
  buffer.put(1);
  REQUIRE(buffer.size() == 1);
  buffer.put(2);
  buffer.put(3);
  buffer.put(4);
  buffer.put(5);
  REQUIRE(buffer.size() == 5);
  REQUIRE(buffer.get() == 1);
  REQUIRE(buffer.size() == 4);
  REQUIRE(buffer.get() == 2);
  buffer.put(6);
  REQUIRE(buffer.get() == 3);
}