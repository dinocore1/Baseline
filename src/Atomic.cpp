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

#include <baseline/Baseline.h>
#include <baseline/Atomic.h>

namespace baseline {

#if defined(__GNUC__) || defined(__clang__)

int32_t atomic_inc( volatile int32_t* ptr )
{
  return __sync_fetch_and_add( ptr, 1 );
}

int32_t atomic_dec( volatile int32_t* ptr )
{
  return __sync_fetch_and_sub( ptr, 1 );
}

int32_t atomic_add( int32_t value, volatile int32_t* ptr )
{
  return __sync_fetch_and_add( ptr, value );
}

int32_t atomic_and( int32_t value, volatile int32_t* ptr )
{
  return __sync_fetch_and_and( ptr, value );
}

int32_t atomic_or( int32_t value, volatile int32_t* ptr )
{
  return __sync_fetch_and_or( ptr, value );
}

int32_t atomic_cmpxchg( int32_t oldvalue, int32_t newvalue, volatile int32_t* ptr )
{
  return __sync_val_compare_and_swap( ptr, oldvalue, newvalue );
}

#elif defined(WIN32)

int32_t atomic_inc( volatile int32_t* ptr )
{
  return InterlockedIncrement( ( long* ) ptr ) - 1;
}

int32_t atomic_dec( volatile int32_t* ptr )
{
  return InterlockedDecrement( ( long* )ptr ) + 1;
}

int32_t atomic_add( int32_t value, volatile int32_t* ptr )
{
  return InterlockedAdd( ( long* )ptr, value );
}

int32_t atomic_and( int32_t value, volatile int32_t* ptr )
{
  return InterlockedAnd( ( long* )ptr, value );
}

int32_t atomic_or( int32_t value, volatile int32_t* ptr )
{
  return InterlockedOr( ( long* )ptr, value );
}

int32_t atomic_cmpxchg( int32_t oldvalue, int32_t newvalue, volatile int32_t* ptr )
{
  return InterlockedCompareExchange( ( long* )ptr, ( long )oldvalue, ( long )newvalue );
}


#endif

}