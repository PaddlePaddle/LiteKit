/*
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
