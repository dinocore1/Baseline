#include <baseline/Baseline.h>
#include <baseline/Atomic.h>

namespace baseline {

#if defined(__GNUC__)

int32_t atomic_inc( int32_t* ptr )
{
  return __sync_fetch_and_add( ptr, 1 );
}

int32_t atomic_dec( int32_t* ptr )
{
  return __sync_fetch_and_sub( ptr, 1 );
}

#endif



}