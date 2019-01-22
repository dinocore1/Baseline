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

#ifndef BASELINE_HASH_H_
#define BASELINE_HASH_H_

#include <baseline/String8.h>
#include <baseline/UniquePointer.h>
#include <baseline/Comparable.h>

namespace baseline {

class SharedBuffer;

class HashCode : public Comparable<HashCode>
{
public:
  HashCode( void* buf, size_t len );
  HashCode( const HashCode& );
  ~HashCode();

  String8 toHexString() const;
  int compare( const HashCode& rhs ) const override;

private:
  SharedBuffer* mBuffer;
};

class HashFunction
{
public:
  virtual ~HashFunction();
  virtual void update( void* buf, size_t len ) = 0;
  virtual HashCode finalize() = 0;

};

/**
 * Create a new HashFunction that computes CRC32.
 */
up<HashFunction> createCRC32();

} // namespace

#endif // BASELINE_HASH_H_