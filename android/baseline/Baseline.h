#ifndef BASELINE_H_
#define BASELINE_H_

#include <log/log.h>

#ifndef ANDROID
  #define ANDROID
#endif

//use Android's utils
#define BASELINE_STRONGPOINTER_H_
#define BASELINE_REFBASE_H_
#define BASELINE_TYPEHELPERS_H_
#define BASELINE_STRING8_H_
#define BASELINE_STRING16_H_
#define BASELINE_VECTOR_H_
#define BASELINE_VECTORIMPL_H_
#define BASELINE_DEBUG_H_
#define BASELINE_TEXTOUTPUT_H_
#include <utils/TypeHelpers.h>
#include <utils/String8.h>
#include <utils/RefBase.h>
#include <utils/StrongPointer.h>
#include <utils/Vector.h>
#include <utils/VectorImpl.h>
#include <utils/Debug.h>


using namespace android;

#ifdef WIN32
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
#endif

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <errno.h>

#define BASELINE_VERSION_MAJOR 0
#define BASELINE_VERSION_MINOR 1
#define BASELINE_VERSION_PATCH 1
#define BASELINE_THREAD_SUPPORT
#define CMAKE_USE_PTHREADS_INIT
/* #undef CMAKE_USE_WIN32_THREADS_INIT */

#ifndef MIN
  #define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

#ifndef MAX
  #define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif

#ifndef PI
  #define PI 3.14159265359
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


#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_DLL
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__ ((dllexport))
    #else
      #define DLL_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #else
    #ifdef __GNUC__
      #define DLL_PUBLIC __attribute__ ((dllimport))
    #else
      #define DLL_PUBLIC __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
    #endif
  #endif
  #define DLL_LOCAL
#else
  #if __GNUC__ >= 4
    #define DLL_PUBLIC __attribute__ ((visibility ("default")))
    #define DLL_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define DLL_PUBLIC
    #define DLL_LOCAL
  #endif
#endif

namespace baseline {

#if defined(BASELINE_THREAD_SUPPORT)
  #if defined(CMAKE_USE_PTHREADS_INIT)
    #include <pthread.h>
    typedef pthread_mutex_t mutex_t;
    typedef pthread_cond_t condition_t;
    typedef pthread_t thread_t;
  #elif defined(CMAKE_USE_WIN32_THREADS_INIT)
    typedef CRITICAL_SECTION mutex_t;
    typedef CONDITION_VARIABLE condition_t;
    typedef unsigned thread_t;
  #endif
#endif

template<class T>
void swap( T& a, T& b )
{
  T c( a );
  a = b;
  b = c;
}

#ifdef WIN32
  #undef NO_ERROR
#endif

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

#ifdef WIN32
  #define NO_ERROR 0L
#endif

}


#endif // BASELINE_H_
