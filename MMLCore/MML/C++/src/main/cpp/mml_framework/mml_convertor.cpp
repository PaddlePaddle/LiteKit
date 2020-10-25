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
