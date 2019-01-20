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

#ifndef BASELINE_LOG_H_
#define BASELINE_LOG_H_

namespace baseline {

enum LogLevel {
  Verbose = 0,
  Debug,
  Info,
  Warning,
  Error,
  Fatal
};

#ifndef LOG_LEVEL
  #define LOG_LEVEL baseline::LogLevel::INFO
#endif

typedef void ( *logfunction_t )( const char* format, va_list args );

void stdOutLogFunction( const char* format, va_list args );

void setLogLevel( LogLevel );
void setLogFunction( logfunction_t );
void log( LogLevel, const char* format, ... );

#define ERROR_TAG "e"
#define WARN_TAG "w"
#define INFO_TAG "i"
#define DEBUG_TAG "d"
#define VERBOSE_TAG "v"

#ifdef WIN32

#define LOG_ARGS(tag, level, format) \
  "%s/%s %s:%d | " format "\n", level, tag, __FILE__, __LINE__

#else

#define LOG_ARGS(tag, level, format) \
  "%s/%s %s:%d | " format "\n", level, tag, basename(__FILE__), __LINE__

#endif

#define LOG_ERROR(tag, format, ...)   \
  log(baseline::LogLevel::Error, LOG_ARGS(tag, ERROR_TAG, format), ## __VA_ARGS__)

#define LOG_WARN(tag, format, ...)   \
  log(baseline::LogLevel::Warning, LOG_ARGS(tag, WARN_TAG, format), ## __VA_ARGS__)

#define LOG_INFO(tag, format, ...)   \
  log(baseline::LogLevel::Info, LOG_ARGS(tag, INFO_TAG, format), ## __VA_ARGS__)

#define LOG_DEBUG(tag, format, ...)   \
  log(baseline::LogLevel::Debug, LOG_ARGS(tag, DEBUG_TAG, format), ## __VA_ARGS__)

#define LOG_VERBOSE(tag, format, ...)   \
  log(baseline::LogLevel::Verbose, LOG_ARGS(tag, VERBOSE_TAG, format), ## __VA_ARGS__)



} // namespace

#define ALOG_ASSERT(cond, ...)
#define LOG_ALWAYS_FATAL_IF(cond, ...)
#define ALOGW_IF(cond, ...)
#define LOG_FATAL_IF(cond, ...)
#define LOG_ALWAYS_FATAL(...)


#endif // BASELINE_LOG_H_