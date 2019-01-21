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
#include <baseline/BaseEncoding.h>
#include <baseline/SharedBuffer.h>

namespace baseline {

String8 BaseEncoding::encode(SharedBuffer* buf) const
{
    return encode(buf->data(), buf->size());
}

class HexBaseEncoding : public BaseEncoding {
public:

    String8 encode(void* buf, size_t len) const override;
    SharedBuffer* decode(const String8&) const override;

};

String8 HexBaseEncoding::encode(void* buf, size_t len) const
{
    static const char dec2hex[16+1] = "0123456789abcdef";
    String8 string;

    for(size_t i=0;i<len;i++) {
        uint8_t value = reinterpret_cast<uint8_t*>(buf)[i];

        string.append(dec2hex[(value >> 4) & 15]);
        string.append(dec2hex[value        & 15]);
    }

    return string;
}

SharedBuffer* HexBaseEncoding::decode(const String8& str) const
{

}

HexBaseEncoding gHexEncoding;

BaseEncoding& hexEncoding()
{
    return gHexEncoding;
}

}
