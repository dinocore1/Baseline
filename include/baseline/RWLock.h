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

#ifndef BASELINE_RWLOCK_H_
#define BASELINE_RWLOCK_H_


namespace baseline {

class RWLock
{
public:
  RWLock();
  ~RWLock();

  status_t readLock();
  status_t writeLock();
  void unlock();

  class AutoRLock
  {
  public:
    inline explicit AutoRLock( RWLock& rwlock ) : mLock( rwlock )  {
      mLock.readLock();
    }
    inline ~AutoRLock() {
      mLock.unlock();
    }
  private:
    RWLock& mLock;
  };

  class AutoWLock
  {
  public:
    inline explicit AutoWLock( RWLock& rwlock ) : mLock( rwlock )  {
      mLock.writeLock();
    }
    inline ~AutoWLock() {
      mLock.unlock();
    }
  private:
    RWLock& mLock;
  };

private:
  // A RWLock cannot be copied
  RWLock( const RWLock& );
  RWLock& operator = ( const RWLock& );

  rwlock_t mRWLock;
#ifdef WIN32
  int mMode;
#endif
};

} // namespace baseline


#endif // BASELINE_RWLOCK_H_