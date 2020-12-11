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


#import <Foundation/Foundation.h>
#import <CoreML/CoreML.h>
#import <CoreVideo/CoreVideo.h>
#import <UIKit/UIKit.h>
#import <Metal/Metal.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * LiteKitTensor date type enum
 */
typedef NS_ENUM(NSInteger, LiteKitTensorDataType) {
    LiteKitTensorDataTypeInt32 = 0,  // int32
    LiteKitTensorDataTypeInt64 = 1,  // int64
    LiteKitTensorDataTypeFloat = 2,  // float32
    LiteKitTensorDataTypeDouble= 3,  // Double
};

/**
 * LiteKitData data type enum
 */
typedef NS_ENUM(NSUInteger, TLiteKitDataType) {
    TLiteKitDataTypeMultiArray = 2,     //MLMultiArray data
    TLiteKitDataTypeCVPixelBuffer = 3,  //CVPixelBuffer data
    TLiteKitDataTypeFloats = 4,         //LiteKitFloat data
    TLiteKitDataTypeLiteKitTensor = 5,  //LiteKitTensor data
    TLiteKitDataTypeImageURL = 6,       //Image地址 data
    TLiteKitDataTypeImage = 7,          //Image data
    TLiteKitDataTypeDictionary = 8,     //NSDictionary data
    TLiteKitDataTypeLiteKitShapedData = 9,   //LiteKitShapedData data
    TLiteKitDataTypeMTLTexture = 10,    //MTLTexture data
    TLiteKitDataTypeMTLBuffer = 11,     //MTLBuffer data
    TLiteKitDataTypeString = 12,        //NSString data
};

/**
 * LiteKitFloat struct，include float*  and length。
 */
typedef struct LiteKitFloat {
    NSUInteger length;// data length
    float *data;// data pointer
} LiteKitFloat;

#pragma mark - LiteKitShapedData

/**
 * LiteKitShapedData data
 */
@interface LiteKitShapedData : NSObject
/// data float*
@property (nonatomic, readonly) float *data;
/// data size
@property (nonatomic, readonly) NSInteger dataSize;
/// data dims
@property (nonatomic, readonly) NSArray <NSNumber *> *dim;

/**
 * init method
 
 @param data init data
 @param dataSize  data length
 @param dim  data dims
 @return init data
 */
- (instancetype)initWithData:(float *)data
                    dataSize:(NSInteger)dataSize
                        dims:(NSArray <NSNumber *> *)dim;
@end

#pragma mark - LiteKitBMLTensor

/**
 * LiteKitBMLTensor data
 */
@interface LiteKitBMLTensor : NSObject
/// data featurearray
@property (nonatomic, readonly) NSArray *dataArray;
///predict threshold
@property (nonatomic, readonly) NSNumber *threshold;
/// data type
@property (nonatomic, readonly) LiteKitTensorDataType dataType;

/**
 * init
 
 @param dataArray  data feature array
 @param threshold predict
 @param dataType  data type
 @return init data
 */
- (instancetype)initWithArray:(NSArray *)dataArray
                    threshold:(NSNumber *)threshold
                     dataType:(LiteKitTensorDataType)dataType;
@end

#pragma mark - LiteKitData

/**
 * LiteKitData define
 */
@interface LiteKitData : NSObject

///LiteKit tensor
@property (nonatomic, strong) LiteKitBMLTensor *litekitTensor;
///dictionary type data
@property (nonatomic, strong) NSDictionary *dictionaryData;
///string type data
@property (nonatomic, strong) NSDictionary *stringData;

///rare float data
@property (nonatomic, assign) LiteKitFloat *floatData;

///MLMultiArray image data ，multiArray support 4 channels data most
@property (nonatomic, strong) MLMultiArray *multiArray;
///CVPixelBufferRef image data
@property (nonatomic, assign) CVPixelBufferRef pixelBuffer;
///image path
@property (nonatomic, strong) NSString *imageURL;
/// image data
@property (nonatomic, strong) UIImage *image;
///LiteKitshapeddata
@property (nonatomic, strong) LiteKitShapedData *litekitShapedData;
///MTLTexture MTL texture input
@property (nonatomic, strong) id<MTLTexture> mtlTexture;
///MTLBuffer MTL buffer input
@property (nonatomic, strong) id<MTLBuffer> mtlBuffer;
///MTLTexture or MTLBuffer will use this propertty , dims rank is n h w c，
@property (nonatomic, strong) NSArray *dims;

@property (nonatomic, assign) TLiteKitDataType type;

/**
 * init
 
 @param data  data
 @param type  data type
 @return init LiteKitData
 */
- (instancetype)initWithData:(id)data type:(TLiteKitDataType)type;

@end


NS_ASSUME_NONNULL_END
