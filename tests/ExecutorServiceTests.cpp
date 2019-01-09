#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <baseline/Baseline.h>
#include <baseline/ExecutorService.h>

using namespace baseline;


TEST_CASE( "one-time runs tasks", "[ExecutorService]" )
{
  static int count = 0;
  class MyRunnable : public Runnable
  {
  public:
    ~MyRunnable() {}
    void run() {
      count++;
    }
  };
  sp<ExecutorService> exe = ExecutorService::createSingleThread( String8( "exe" ) );
  sp<Future> f = exe->execute( new MyRunnable );
  f->wait();

  REQUIRE( count == 1 );
  exe->shutdown();
}


TEST_CASE( "repeating runs tasks", "[ExecutorService]" )
{
  static int count = 0;
  static sp<Future> f;
  sp<ExecutorService> exe = ExecutorService::createSingleThread( String8( "exe" ) );

  class MyRunnable : public Runnable
  {
  public:
    ~MyRunnable() {}
    void run() {
      count++;
      if( count == 5 ) {
        f->cancel();
      }
    }
  };


  f = exe->scheduleWithFixedDelay( new MyRunnable, 100 );
  f->wait();

  REQUIRE( count == 5 );
  exe->shutdown();
}