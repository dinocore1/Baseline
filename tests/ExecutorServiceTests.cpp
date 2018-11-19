#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <baseline/Baseline.h>
#include <baseline/ExecutorService.h>

using namespace baseline;

TEST_CASE( "runs tasks", "[ExecutorService]" )
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
  exe->execute( new MyRunnable );

  exe->shutdown();


}