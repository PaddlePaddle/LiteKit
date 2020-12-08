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


#import "LiteKitData.h"

API_AVAILABLE(ios(11.0))


@implementation LiteKitData

#pragma mark init
- (instancetype)initWithData:(id)data type:(TLiteKitDataType)type {
    self = [super init];
    if (self) {
        [self setData:data withType:type];
    }
    return self;
}

#pragma mark setter
- (void)setMultiArray:(MLMultiArray *)multiArray {
    if (![multiArray isKindOfClass:[MLMultiArray class]]) {
        return;
    }
    
    _multiArray = multiArray;
    _type = TLiteKitDataTypeMultiArray;
}

- (void)setPixelBuffer:(CVPixelBufferRef)pixelBuffer {
    _pixelBuffer = pixelBuffer;
    _type = TLiteKitDataTypeCVPixelBuffer;
}

- (void)setFloatData:(LiteKitFloat *)floatData {
    if (_floatData != NULL) {
        freeLiteKitFloat(_floatData);
        free(_floatData);
    }
    
    _floatData = (LiteKitFloat *)malloc(sizeof(LiteKitFloat));
    _floatData->length = floatData->length;
    _floatData->data = floatData->data;
    
    _type = TLiteKitDataTypeFloats;
}

- (void)setImageURL:(NSString *)imageURL {
    if (![imageURL isKindOfClass:[NSString class]]) {
        return;
    }
    _imageURL = imageURL;
    _type = TLiteKitDataTypeImageURL;
}

- (void)setImage:(UIImage *)image {
    if (![image isKindOfClass:[UIImage class]]) {
        return;
    }
    _image = image;
    _type = TLiteKitDataTypeImage;
}

- (void)setDictionaryData:(NSDictionary<NSString *, NSNumber *> *)dictionaryData {
    if (![dictionaryData isKindOfClass:[NSDictionary class]]) {
        return;
    }
    _dictionaryData = dictionaryData;
    _type = TLiteKitDataTypeDictionary;
}

- (void)setLitekitShapedData:(LiteKitShapedData *)litekitShapedData {
    if (![litekitShapedData isKindOfClass:[LiteKitShapedData class]]) {
        return;
    }
    _litekitShapedData = litekitShapedData;
    _type = TLiteKitDataTypeLiteKitShapedData;
}

- (void)setMtlTexture:(id<MTLTexture>)mtlTexture {
    _mtlTexture = mtlTexture;
    _type = TLiteKitDataTypeMTLTexture;
}

- (void)setMtlBuffer:(id<MTLBuffer>)mtlBuffer {
    _mtlBuffer = mtlBuffer;
    _type = TLiteKitDataTypeMTLBuffer;
}

- (void)setStringData:(NSDictionary *)stringData {
    _stringData = stringData;
    _type = TLiteKitDataTypeString;
}

- (void)setLitekitTensor:(LiteKitBMLTensor *)litekitTensor {
    _litekitTensor = litekitTensor;
    _type = TLiteKitDataTypeLiteKitTensor;
}

- (void)setData:(id)data withType:(TLiteKitDataType)type {
    switch (type) {
        case TLiteKitDataTypeMultiArray:
            self.multiArray = data;
            break;
            
        case TLiteKitDataTypeCVPixelBuffer:
            self.pixelBuffer = (__bridge CVPixelBufferRef)data;
            break;
            
        case TLiteKitDataTypeFloats:
            self.floatData = (__bridge LiteKitFloat *)data;
            break;
            
        case TLiteKitDataTypeLiteKitTensor:
            self.litekitTensor = data;
            break;
            
        case TLiteKitDataTypeImageURL:
            self.imageURL = data;
            break;
            
        case TLiteKitDataTypeImage:
            self.image = data;
            break;
            
        case TLiteKitDataTypeDictionary:
            self.dictionaryData = data;
            break;
            
        case TLiteKitDataTypeLiteKitShapedData:
            self.litekitShapedData = data;
            break;
            
        case TLiteKitDataTypeMTLTexture:
            self.mtlTexture = data;
            break;
            
        case TLiteKitDataTypeMTLBuffer:
            self.mtlBuffer = data;
            break;
        
        case TLiteKitDataTypeString:
            self.stringData = data;
            break;
            
        default:
            break;
    }
}

-(void)dealloc {
    freeLiteKitFloat(self.floatData);
    free(self.floatData);
}

void freeLiteKitFloat(LiteKitFloat *floatData) {
    if (NULL != floatData) {
        free(floatData->data);
    }
}

@end


#pragma mark - LiteKitShapedData
@interface LiteKitShapedData ()
@property (nonatomic, assign)float *data;
@property (nonatomic, assign)NSInteger dataSize;
@property (nonatomic, strong) NSArray <NSNumber *> *dim;
@end
@implementation LiteKitShapedData

- (instancetype)initWithData:(float *)data
                    dataSize:(NSInteger)dataSize
                        dims:(NSArray <NSNumber *> *)dim {
    self = [super init];
    if (self) {
        _data = (float *)malloc(dataSize * sizeof(float));
        memcpy(_data, data, dataSize * sizeof(float));
        _dataSize = dataSize;
        _dim = [dim copy];
    }
    return self;
}

- (void)dealloc {
    if (_data != NULL) {
        free(_data);
    }
}

@end


#pragma mark - LiteKitBMLTensor
@interface LiteKitBMLTensor ()
@property (nonatomic, strong) NSArray *dataArray;
@property (nonatomic, strong) NSNumber *threshold;
@property (nonatomic, assign) LiteKitTensorDataType dataType;
@end
@implementation LiteKitBMLTensor

- (instancetype)initWithArray:(NSArray *)dataArray
                   threshold:(NSNumber *)threshold
                     dataType:(LiteKitTensorDataType)dataType {
    self = [super init];
    if (self) {
        _dataArray = dataArray;
        _threshold = threshold;
        _dataType = dataType;
    }
    return self;
}

@end
