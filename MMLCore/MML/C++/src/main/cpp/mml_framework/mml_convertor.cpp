// Copyright (c) 2019 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "mml_convertor.h"

#include <stdint.h>

using namespace mml_framework;

int sizeOfRawDataType(MMLData::RawDataType mmlDataType) {
    switch (mmlDataType)
    {
        case MMLData::RawDataType::FLOAT32:
            return sizeof(float);
        case MMLData::RawDataType::UINT8:
            return sizeof(uint8_t);

        default:
            return sizeof(float);
    }
}

void* createRawData(MMLData::RawDataType mmlDataType, int len) {
    switch (mmlDataType)
    {
        case MMLData::RawDataType::FLOAT32:
            return new float[len];
        case MMLData::RawDataType::UINT8:
            return new uint8_t[len];

        default:
            return new float[len];
    }
}
