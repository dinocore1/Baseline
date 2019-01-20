/*
 * Copyright (C) 2018 Baseline
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
 */

#include <baseline/Baseline.h>
#include <baseline/Log.h>

#include <cstdio>

#define LOG_LINE_SIZE 1024

namespace baseline {

LogLevel gLogLevel = LogLevel::Info;
logfunction_t gLogFunction = stdOutLogFunction;

void setLogLevel( LogLevel level )
{
  gLogLevel = level;
}

void setLogFunction( logfunction_t f )
{
  gLogFunction = f;
}

void stdOutLogFunction( const char* format, va_list args )
{
  vfprintf( stderr, format, args );
}

void log( LogLevel level, const char* format, ... )
{
  va_list args;
  va_start( args, format );


  if( level >= gLogLevel ) {
    gLogFunction( format, args );
  }

  va_end( args );
}

} // namespace