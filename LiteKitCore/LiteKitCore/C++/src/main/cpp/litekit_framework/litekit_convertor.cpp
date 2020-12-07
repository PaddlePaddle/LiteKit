/**
Copyright © 2020 Baidu, Inc. All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "litekit_convertor.h"

#include <stdint.h>

using namespace litekit_framework;

int sizeOfRawDataType(LiteKitData::RawDataType litekitDataType) {
    switch (litekitDataType)
    {
        case LiteKitData::RawDataType::FLOAT32:
            return sizeof(float);
        case LiteKitData::RawDataType::UINT8:
            return sizeof(uint8_t);

        default:
            return sizeof(float);
    }
}

void* createRawData(LiteKitData::RawDataType litekitDataType, int len) {
    switch (litekitDataType)
    {
        case LiteKitData::RawDataType::FLOAT32:
            return new float[len];
        case LiteKitData::RawDataType::UINT8:
            return new uint8_t[len];

        default:
            return new float[len];
    }
}
