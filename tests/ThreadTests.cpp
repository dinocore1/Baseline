#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <baseline/Baseline.h>
#include <baseline/Thread.h>

using namespace baseline;

TEST_CASE( "thread runs", "[Thread]" )
{

  static int count = 0;

  class MyThread : public Thread
  {
  public:
    void run() {
      //I am running in a new thread
      count++;
    }
  };

  MyThread* t = new MyThread();
  t->start();
  t->join();
  REQUIRE( count == 1 );

  //no need to call delete, the thread will clean it self up when it finishes
  //delete t;

}