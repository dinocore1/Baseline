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

#ifndef ATOMIC_H_
#define ATOMIC_H_

namespace baseline {

/**
 * atomic fetch and increment 1
 * @return previous value
 */
int32_t atomic_inc( volatile int32_t* ptr );

/**
 * atomic fetch and decrement 1
 * @return previous value
 */
int32_t atomic_dec( volatile int32_t* ptr );

int32_t atomic_add( int32_t value, volatile int32_t* ptr );

int32_t atomic_and( int32_t value, volatile int32_t* ptr );

int32_t atomic_or( int32_t value, volatile int32_t* ptr );

int32_t atomic_cmpxchg( int32_t oldvalue, int32_t newvalue, volatile int32_t* ptr );

}


#endif // ATOMIC_H_