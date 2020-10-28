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


#import "MMLData.h"

API_AVAILABLE(ios(11.0))


@implementation MMLData

#pragma mark init
- (instancetype)initWithData:(id)data type:(TMMLDataType)type {
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
    _type = TMMLDataTypeMultiArray;
}

- (void)setPixelBuffer:(CVPixelBufferRef)pixelBuffer {
    _pixelBuffer = pixelBuffer;
    _type = TMMLDataTypeCVPixelBuffer;
}

- (void)setFloatData:(MMLFloat *)floatData {
    if (_floatData != NULL) {
        freeMMLFloat(_floatData);
        free(_floatData);
    }
    
    _floatData = (MMLFloat *)malloc(sizeof(MMLFloat));
    _floatData->length = floatData->length;
    _floatData->data = floatData->data;
    
    _type = TMMLDataTypeFloats;
}

- (void)setImageURL:(NSString *)imageURL {
    if (![imageURL isKindOfClass:[NSString class]]) {
        return;
    }
    _imageURL = imageURL;
    _type = TMMLDataTypeImageURL;
}

- (void)setImage:(UIImage *)image {
    if (![image isKindOfClass:[UIImage class]]) {
        return;
    }
    _image = image;
    _type = TMMLDataTypeImage;
}

- (void)setDictionaryData:(NSDictionary<NSString *, NSNumber *> *)dictionaryData {
    if (![dictionaryData isKindOfClass:[NSDictionary class]]) {
        return;
    }
    _dictionaryData = dictionaryData;
    _type = TMMLDataTypeDictionary;
}

- (void)setMmlShapedData:(MMLShapedData *)mmlShapedData {
    if (![mmlShapedData isKindOfClass:[MMLShapedData class]]) {
        return;
    }
    _mmlShapedData = mmlShapedData;
    _type = TMMLDataTypeMMLShapedData;
}

- (void)setMtlTexture:(id<MTLTexture>)mtlTexture {
    _mtlTexture = mtlTexture;
    _type = TMMLDataTypeMTLTexture;
}

- (void)setMtlBuffer:(id<MTLBuffer>)mtlBuffer {
    _mtlBuffer = mtlBuffer;
    _type = TMMLDataTypeMTLBuffer;
}

- (void)setStringData:(NSDictionary *)stringData {
    _stringData = stringData;
    _type = TMMLDataTypeString;
}

- (void)setMmlTensor:(MMLBMLTensor *)mmlTensor {
    _mmlTensor = mmlTensor;
    _type = TMMLDataTypeMMLTensor;
}

- (void)setData:(id)data withType:(TMMLDataType)type {
    switch (type) {
        case TMMLDataTypeMultiArray:
            self.multiArray = data;
            break;
            
        case TMMLDataTypeCVPixelBuffer:
            self.pixelBuffer = (__bridge CVPixelBufferRef)data;
            break;
            
        case TMMLDataTypeFloats:
            self.floatData = (__bridge MMLFloat *)data;
            break;
            
        case TMMLDataTypeMMLTensor:
            self.mmlTensor = data;
            break;
            
        case TMMLDataTypeImageURL:
            self.imageURL = data;
            break;
            
        case TMMLDataTypeImage:
            self.image = data;
            break;
            
        case TMMLDataTypeDictionary:
            self.dictionaryData = data;
            break;
            
        case TMMLDataTypeMMLShapedData:
            self.mmlShapedData = data;
            break;
            
        case TMMLDataTypeMTLTexture:
            self.mtlTexture = data;
            break;
            
        case TMMLDataTypeMTLBuffer:
            self.mtlBuffer = data;
            break;
        
        case TMMLDataTypeString:
            self.stringData = data;
            break;
            
        default:
            break;
    }
}

-(void)dealloc {
    freeMMLFloat(self.floatData);
    free(self.floatData);
}

void freeMMLFloat(MMLFloat *floatData) {
    if (NULL != floatData) {
        free(floatData->data);
    }
}

@end


#pragma mark - MMLShapedData
@interface MMLShapedData ()
@property (nonatomic, assign)float *data;
@property (nonatomic, assign)NSInteger dataSize;
@property (nonatomic, strong) NSArray <NSNumber *> *dim;
@end
@implementation MMLShapedData

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


#pragma mark - MMLBMLTensor
@interface MMLBMLTensor ()
@property (nonatomic, strong) NSArray *dataArray;
@property (nonatomic, strong) NSNumber *threshold;
@property (nonatomic, assign) MMLTensorDataType dataType;
@end
@implementation MMLBMLTensor

- (instancetype)initWithArray:(NSArray *)dataArray
                   threshold:(NSNumber *)threshold
                     dataType:(MMLTensorDataType)dataType {
    self = [super init];
    if (self) {
        _dataArray = dataArray;
        _threshold = threshold;
        _dataType = dataType;
    }
    return self;
}

@end
