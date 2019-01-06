/*
 * Copyright (C) 2007 The Android Open Source Project
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
 * Modifications by Paul Soucy copyright (C) 2018
 */

#ifndef BASELINE_MUTEX_H_
#define BASELINE_MUTEX_H_

namespace baseline {

class Mutex
{
public:
  Mutex();
  ~Mutex();
  status_t lock();
  void unlock();
  status_t tryLock();

  class Autolock
  {
  public:
    inline Autolock( Mutex& mutex ) : mLock( mutex ) {
      mLock.lock();
    }
    inline Autolock( Mutex* mutex ) : mLock( *mutex ) {
      mLock.lock();
    }
    inline ~Autolock() {
      mLock.unlock();
    }
  private:
    Mutex& mLock;
  };

private:
  friend class Condition;

  //A mutex cannot be copied
  Mutex( const Mutex& );
  Mutex& operator = ( const Mutex& );

  mutex_t mMutex;
};

}

#endif // BASELINE_MUTEX_H_