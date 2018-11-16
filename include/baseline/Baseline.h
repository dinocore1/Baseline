#ifndef BASELINE_H_
#define BASELINE_H_

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <errno.h>


#ifndef MIN
  #define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef MAX
  #define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifdef WIN32
  /* Define the default path separator for the platform. */
  #define OS_PATH_SEPARATOR     '\\'
  #define OS_PATH_SEPARATOR_STR "\\"

#else
  /* Define the default path separator for the platform. */
  #define OS_PATH_SEPARATOR     '/'
  #define OS_PATH_SEPARATOR_STR "/"

#endif

namespace baseline {

template<class T>
void swap( T& a, T& b )
{
  T c( a );
  a = b;
  b = c;
}

typedef int32_t status_t;
enum {
  OK                = 0,    // Everything's swell.
  NO_ERROR          = 0,    // No errors.

  UNKNOWN_ERROR       = 0x80000000,

  NO_MEMORY           = -ENOMEM,
  INVALID_OPERATION   = -ENOSYS,
  BAD_VALUE           = -EINVAL,
  BAD_TYPE            = 0x80000001,
  NAME_NOT_FOUND      = -ENOENT,
  PERMISSION_DENIED   = -EPERM,
  NO_INIT             = -ENODEV,
  ALREADY_EXISTS      = -EEXIST,
  DEAD_OBJECT         = -EPIPE,
  FAILED_TRANSACTION  = 0x80000002,
  JPARKS_BROKE_IT     = -EPIPE,
#if !defined(HAVE_MS_C_RUNTIME)
  BAD_INDEX           = -EOVERFLOW,
  NOT_ENOUGH_DATA     = -ENODATA,
  WOULD_BLOCK         = -EWOULDBLOCK,
  TIMED_OUT           = -ETIMEDOUT,
  UNKNOWN_TRANSACTION = -EBADMSG,
#else
  BAD_INDEX           = -E2BIG,
  NOT_ENOUGH_DATA     = 0x80000003,
  WOULD_BLOCK         = 0x80000004,
  TIMED_OUT           = 0x80000005,
  UNKNOWN_TRANSACTION = 0x80000006,
#endif
  FDS_NOT_ALLOWED     = 0x80000007,
};


}

#include <baseline/Log.h>

#endif // BASELINE_H_