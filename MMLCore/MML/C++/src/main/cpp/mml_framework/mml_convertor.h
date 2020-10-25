
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
