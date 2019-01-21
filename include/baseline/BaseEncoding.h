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

#ifndef BASELINE_BASEENCODING_H_
#define BASELINE_BASEENCODING_H_

#include <baseline/String8.h>

namespace baseline {

class SharedBuffer;

class BaseEncoding {
public:
    String8 encode(SharedBuffer*) const;
    virtual String8 encode(void* buf, size_t len) const = 0;
    virtual SharedBuffer* decode(const String8&) const = 0;

};

BaseEncoding& hexEncoding();

} // namespace

#endif // BASELINE_BASEENCODING_H_