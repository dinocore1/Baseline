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

#include <Windows.h>

  int32_t atomic_inc(volatile int32_t* ptr)
  {
    return InterlockedIncrement( (long*) ptr) - 1;
  }

  int32_t atomic_dec(volatile int32_t* ptr)
  {
    return InterlockedDecrement((long*)ptr) + 1;
  }

  int32_t atomic_add(int32_t value, volatile int32_t* ptr)
  {
    return InterlockedAdd((long*)ptr, value);
  }

  int32_t atomic_and(int32_t value, volatile int32_t* ptr)
  {
    return InterlockedAnd((long*)ptr, value);
  }

  int32_t atomic_or(int32_t value, volatile int32_t* ptr)
  {
    return InterlockedOr((long*)ptr, value);
  }

  int32_t atomic_cmpxchg(int32_t oldvalue, int32_t newvalue, volatile int32_t* ptr)
  {
    return InterlockedCompareExchange((long*)ptr, (long)oldvalue, (long)newvalue);
  }


#endif

}