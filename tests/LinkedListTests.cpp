#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <baseline/LinkedList.h>

using namespace baseline;

TEST_CASE("add back to list", "[LinkedList]")
{
    LinkedList<int> mylist;
    REQUIRE(mylist.size() == 0);
    mylist.add(5);
    REQUIRE(mylist.size() == 1);
    REQUIRE(mylist.head() == 5);
}