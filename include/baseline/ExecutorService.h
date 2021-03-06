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

#ifndef BASELINE_EXECUTORSERVICE_H_
#define BASELINE_EXECUTORSERVICE_H_

#include <baseline/String8.h>
#include <baseline/StrongPointer.h>
#include <baseline/RefBase.h>

namespace baseline {

class Runnable : public RefBase
{
public:
  virtual ~Runnable() {};
  virtual void run() = 0;
};

class Future : public RefBase
{
public:
  virtual void wait();
  virtual void cancel();
};

class ExecutorService : public RefBase
{
public:

  static sp<ExecutorService> createExecutorService( const String8& name, int numThreads = 1 );

  static inline sp<ExecutorService> createSingleThreadedExecutorService( const String8& name ) {
    return createExecutorService( name, 1 );
  }

  /**
   * Cancels and queued tasks and waits for any currently running tasks to finish.
   */
  virtual void shutdown() = 0;

  /**
   * execute a one-time
   */
  virtual sp<Future> execute( const sp<Runnable>& ) = 0;

  /**
   * schedule one-time task to run in the future
   */
  virtual sp<Future> schedule( const sp<Runnable>&, uint32_t delayMS ) = 0;

  /**
   * schedule a re-occuring task to with a fixed delay between execution
   */
  virtual sp<Future> scheduleWithFixedDelay( const sp<Runnable>&, uint32_t delayMS ) = 0;

};

}

#endif // BASELINE_EXECUTORSERVICE_H_