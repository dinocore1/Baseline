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

#ifndef BASELINE_CONDITION_H_
#define BASELINE_CONDITION_H_

#include <baseline/Mutex.h>

namespace baseline {

class Condition
{
public:
  Condition();
  ~Condition();

  status_t wait( Mutex& mutex );
//status_t waitRelative(Mutex& mutex, nsecs_t reltime);

  /**
   * Signal one thread to wakeup
   */
  void signalOne();

  /**
   * Signal all threads to wakeup
   */
  void signalAll();

private:

  //A Condition cannot be copied
  Condition( const Condition& );
  Condition& operator= ( const Condition& );

  condition_t mVar;

};

}

#endif // BASELINE_CONDITION_H_