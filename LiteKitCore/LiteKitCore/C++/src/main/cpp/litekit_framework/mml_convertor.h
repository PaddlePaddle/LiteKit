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


#ifndef LIB_AI_MML_CONVERTOR_H
#define LIB_AI_MML_CONVERTOR_H
#include "mml_inference_api.h"

using namespace mml_framework;

/**
 * 返回MMLDataType的Size
 * @param mmlDataType
 * @return
 */
int sizeOfRawDataType(MMLData::RawDataType mmlDataType);

/**
 * 根据DataType创建对应的Data数组
 * @param mmlDataType
 * @param len 单位为sizeOfRawDataType()，而不是byte
 * @return
 */
void* createRawData(MMLData::RawDataType mmlDataType, int len);

#endif //LIB_AI_MML_CONVERTOR_H
