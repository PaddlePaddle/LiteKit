//
//  MMLData.h
//  MML+Plugin
//
//  Created by Baidu. Co.,Ltd. on 2019/11/13.
//  Copyright © 2019 Baidu. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreML/CoreML.h>
#import <CoreVideo/CoreVideo.h>
#import <UIKit/UIKit.h>
#import <Metal/Metal.h>

NS_ASSUME_NONNULL_BEGIN

/**
 * MMLTensor数据 数据类型定义
 */
typedef NS_ENUM(NSInteger, MMLTensorDataType) {
    MMLTensorDataTypeInt32 = 0,  // int32
    MMLTensorDataTypeInt64 = 1,  // int64
    MMLTensorDataTypeFloat = 2,  // float32
    MMLTensorDataTypeDouble= 3,  // Double
};

/**
 * MMLData 的数据类型定义
 */
typedef NS_ENUM(NSUInteger, TMMLDataType) {
    TMMLDataTypeMultiArray = 2,     //MLMultiArray数据
    TMMLDataTypeCVPixelBuffer = 3,  //CVPixelBuffer数据
    TMMLDataTypeFloats = 4,         //MMLFloat数据
    TMMLDataTypeMMLTensor = 5,      //MMLTensor数据
    TMMLDataTypeImageURL = 6,       //Image地址数据
    TMMLDataTypeImage = 7,          //Image数据
    TMMLDataTypeDictionary = 8,     //NSDictionary数据
    TMMLDataTypeMMLShapedData = 9,   //MMLShapedData数据
    TMMLDataTypeMTLTexture = 10,    //MTLTexture数据
    TMMLDataTypeMTLBuffer = 11,     //MTLBuffer数据
    TMMLDataTypeString = 12,        //NSString数据
};

/**
 * MMLFloat结构定义，包含float* 和length。
 */
typedef struct MMLFloat {
    NSUInteger length;//数据长度
    float *data;//数据指针
} MMLFloat;

#pragma mark - MMLShapedData

/**
 * MMLShapedData数据
 */
@interface MMLShapedData : NSObject
///数据float*
@property (nonatomic, readonly) float *data;
///数据size
@property (nonatomic, readonly) NSInteger dataSize;
///数据dims
@property (nonatomic, readonly) NSArray <NSNumber *> *dim;

/**
 * 初始化方法
 
 @param data 初始化数据
 @param dataSize 数据长度
 @param dim 数据维度
 @return 初始化的数据
 */
- (instancetype)initWithData:(float *)data
                    dataSize:(NSInteger)dataSize
                        dims:(NSArray <NSNumber *> *)dim;
@end

#pragma mark - MMLBMLTensor

/**
 * MMLBMLTensor数据
 */
@interface MMLBMLTensor : NSObject
///数据featurearray
@property (nonatomic, readonly) NSArray *dataArray;
///predict阈值
@property (nonatomic, readonly) NSNumber *threshold;
///数据类型
@property (nonatomic, readonly) MMLTensorDataType dataType;

/**
 * 初始化方法
 
 @param dataArray 数据feature array
 @param threshold predict
 @param dataType 数据类型
 @return 初始化的数据
 */
- (instancetype)initWithArray:(NSArray *)dataArray
                    threshold:(NSNumber *)threshold
                     dataType:(MMLTensorDataType)dataType;
@end

#pragma mark - MMLData

/**
 * MMLData定义
 */
@interface MMLData : NSObject

///MML tensor 一般用于BML
@property (nonatomic, strong) MMLBMLTensor *mmlTensor;
///dictionary类型数据，一般用于BML结果返回
@property (nonatomic, strong) NSDictionary *dictionaryData;
///string类型数据，一般用于BML结果返回
@property (nonatomic, strong) NSDictionary *stringData;

///rare float数据
@property (nonatomic, assign) MMLFloat *floatData;

///MLMultiArray保存图像数据，multiArray最多只能支持4通道图像数据
@property (nonatomic, strong) MLMultiArray *multiArray;
///CVPixelBufferRef保存图像数据
@property (nonatomic, assign) CVPixelBufferRef pixelBuffer;
///图片地址
@property (nonatomic, strong) NSString *imageURL;
///图片数据画
@property (nonatomic, strong) UIImage *image;
///MMLshapeddata
@property (nonatomic, strong) MMLShapedData *mmlShapedData;
///MTLTexture MTL的texture类型input
@property (nonatomic, strong) id<MTLTexture> mtlTexture;
///MTLBuffer MTL的buffer类型input
@property (nonatomic, strong) id<MTLBuffer> mtlBuffer;
///MTLTexture or MTLBuffer的时候会使用该属性, dims排列是 n h w c，
@property (nonatomic, strong) NSArray *dims;

@property (nonatomic, assign) TMMLDataType type;

/**
 * 初始化方法
 
 @param data 数据
 @param type 数据类型
 @return 初始化的MMLData
 */
- (instancetype)initWithData:(id)data type:(TMMLDataType)type;

@end


NS_ASSUME_NONNULL_END
