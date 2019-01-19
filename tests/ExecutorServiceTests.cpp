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
  sp<Future> f = exe->execute( new MyRunnable );
  f->wait();

  REQUIRE( count == 1 );
  exe->shutdown();
}