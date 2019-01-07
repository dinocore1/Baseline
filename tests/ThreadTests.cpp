#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <baseline/Baseline.h>
#include <baseline/Thread.h>
#include <baseline/Condition.h>
#include <baseline/Mutex.h>

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

  sp<MyThread> t( new MyThread() );
  t->start();
  t->join();
  REQUIRE( count == 1 );

}

TEST_CASE( "condition var timeout returns TIMED_OUT", "[Condition]")
{
  Mutex mutex;
  Condition condition;

  Mutex::Autolock l( mutex );
  status_t retval = condition.waitTimeout(mutex, 500);

  REQUIRE( retval == TIMED_OUT );
}

TEST_CASE( "condition var timeout return OK when signaled before timeout", "[Condition]")
{
  static Mutex mutex;
  static Condition condition;
  status_t retval;

  class MyThread : public Thread
  {
  public:
    void run() {
      sleep(1);
      Mutex::Autolock l( mutex );
      condition.signalOne();
    }
  };

  sp<MyThread> t( new MyThread() );
  t->start();

  {
    Mutex::Autolock l( mutex );
    retval = condition.waitTimeout(mutex, 20000);
  }

  t->join();

  REQUIRE( retval == OK );
}