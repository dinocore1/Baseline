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
#include <baseline/Thread.h>
#include <baseline/Condition.h>
#include <baseline/Mutex.h>

using namespace baseline;

TEST_CASE( "mutex is reentrant", "[Mutex]" )
{
  Mutex m;
  m.lock();
  m.lock();
  m.unlock();
  m.unlock();
}

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

TEST_CASE( "condition var timeout returns TIMED_OUT", "[Condition]" )
{
  Mutex mutex;
  Condition condition;

  Mutex::Autolock l( mutex );
  status_t retval = condition.waitTimeout( mutex, 100 );
  REQUIRE( retval == TIMED_OUT );
}

TEST_CASE( "condition var timeout return OK when signaled before timeout", "[Condition]" )
{
  static Mutex mutex;
  static Condition condition;
  status_t retval;

  class MyThread : public Thread
  {
  public:
    void run() {
      Thread::sleep( 1000 );
      Mutex::Autolock l( mutex );
      condition.signalOne();
    }
  };

  sp<MyThread> t( new MyThread() );
  t->start();

  {
    Mutex::Autolock l( mutex );
    retval = condition.waitTimeout( mutex, 20000 );
  }

  t->join();

  REQUIRE( retval == OK );
}