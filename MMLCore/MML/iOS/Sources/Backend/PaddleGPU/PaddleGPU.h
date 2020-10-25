/* Copyright (c) 2018 PaddlePaddle Authors. All Rights Reserved.
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License. */

#import <Metal/Metal.h>
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

typedef enum : NSUInteger {
    SuperResolutionNetType, // 超分 Net
    MobileNetCombileType,   // mobile Net
    YoloNetType,            // Yolo  Net
    CustomNetType,          // 自定义 Net，由 ModelConfig 指定 Net 参数
} NetType;

typedef enum : NSUInteger {
    PrecisionTypeFloat32 = 1,
    PrecisionTypeFloat16 = 2
} PrecisionType;

typedef enum : NSUInteger {
    LoadMetalInPaddleGPU   = 1,           // 使用 paddleGPU 中的 metal 代码
    LoadMetalInDefaultLib     = 2,     // 使用 main bundle 中的 metal 代码
    LoadMetalInCustomMetalLib = 3      // 使用 metal 库文件
} MetalLoadType;

@interface PaddleGPUResult: NSObject

@property (assign, nonatomic) float *output;

@property (assign, nonatomic) int outputSize;

@property (strong, nonatomic) NSArray <NSNumber *>*dim;

-(void)releaseOutput;

@end

@interface PaddleGPUConfig: NSObject


/*
 * 输入维度信息  [n c h w]
 */
@property (strong, nonatomic) NSArray<NSNumber *> *dims;


/*
 * 模型参数内存地址
 */
@property (assign, nonatomic) void *paramPointer;

/*
 * 模型参数占用内存大小 (kb)
 */
@property (assign, nonatomic) int paramSize;

/*
 * 模型内存地址
 */
@property (assign, nonatomic) void *modelPointer;

/*
 * 模型占用内存大小 (kb)
 */
@property (assign, nonatomic) int modelSize;

/**
 是否使用 MetalPerformanceShaders 进行运算, 运算精度为 32 位时不支持开启该选项, 默认值是 false
 */
@property (assign, nonatomic) BOOL useMPS;

/**
 是否使用最高等级的加速策略, 可能会导致计算精度略微下降, 默认值是 false
 */
@property (assign, nonatomic) BOOL useAggressiveOptimization;

/**
 模型精度
 */
@property (assign, nonatomic) PrecisionType modelPrecision;

/**
 metallib的加载方式, 当 metalLoadType 为LoadMetalInCustomMetalLib 时， metal library 路径不能为空
 */
@property (assign, nonatomic) MetalLoadType metalLoadType;

/**
 metal lib路径
 */
@property (copy, nonatomic) NSString *metalLibPath;


/**
 运算精度， runner 生命周期中不可变
 */
@property (assign, nonatomic) PrecisionType computePrecision;

/**
 是否打开调试信息
 */
@property (assign, nonatomic) BOOL enableDebug;


@end

@interface PaddleGPU: NSObject

/*
 * 初始化
 */
-(instancetype)initWithCommandQueue:(id<MTLCommandQueue>)queue net:(NetType)netType modelConfig:(PaddleGPUConfig *)config;


/**
 对runner加载

 @return 加载结果
 */
-(BOOL)load;


/**
 对runner加载
 
 @param error 错误信息
 */
- (void)loadWithError:(NSError *__autoreleasing  _Nullable * _Nullable)error;



/**
 更新dim
 
 @param dims 待更新的dims信息
 */
- (BOOL)updateInputDim:(NSArray <NSNumber *>*)dims ;


/**
 更新dim
 
 @param dims 待更新的dims信息
 @param error 错误信息
 */
- (void)updateInputDimInDim:(NSArray <NSNumber *>*)dims error:(NSError *__autoreleasing  _Nullable * _Nullable)error;


/**
 根据imageRef获取texture
 
 @param image imageref
 @param block texture回调
 */
- (void)getTextureWithImage:(CGImageRef)image getTexture:(void (^) (id<MTLTexture>))block;

/**
 根据MTLBuffer获取texture，通道数为1
 
 @param buffer MTLBuffer
 @param block texture回调
 */
- (void)getTextureWithBuffer:(id<MTLBuffer>)buffer getTexture:(void (^) (id<MTLTexture>))block;

/**
 根据MTLBuffer获取texture
 
 @param buffer MTLBuffer
 @param block texture回调
 @param channelNum 通道数
 */
- (void)getTextureWithBuffer:(id<MTLBuffer>)buffer getTexture:(void (^) (id<MTLTexture>))block channelNum:(NSInteger)channelNum;

/*
 * texture:     需要进行预测的图像转换的 texture
 * completion:  预测完成回调
 */
-(void)predict:(id<MTLTexture>)texture withCompletion:(void (^)(BOOL, NSArray<NSArray <NSNumber *>*> *))completion;

/**
 带error信息的预测，output为原始数据
 
 @param texture 需要进行预测的图像转换的 texture
 @param completion 预测完成回调
 */
-(void)predict:(id<MTLTexture>)texture withErrorCompletion:(void (^)(NSError * _Nullable, NSArray<NSArray <NSNumber *>*> *))completion;


/*
 * texture:     需要进行预测的图像转换的 texture
 * completion:  预测完成回调
 */
-(void)predict:(id<MTLTexture>)texture withResultCompletion:(void (^)(BOOL, NSArray <PaddleGPUResult *> *))completion;

/**
 带error信息的预测，output封装成PaddleGPUResult
 
 @param texture 需要进行预测的图像转换的 texture
 @param completion 预测完成回调
 */
-(void)predict:(id<MTLTexture>)texture withErrorResultCompletion:(void (^)(NSError * _Nullable, NSArray <PaddleGPUResult *> *))completion;

/**
通过buffer进行预测，output封装成PaddleGPUResult
 
@param buffer     内存buffer
@param channelNum  通道数
@param completion  预测完成回调
*/
- (void)predictWithBuffer:(id<MTLBuffer>)buffer channelNum:(NSInteger)channelNum withResultCompletion:(void (^)(BOOL, NSArray <PaddleGPUResult *> *))completion;

/**
通过buffer进行预测，output封装成PaddleGPUResult

@param buffer 内存buffer
@param channelNum  通道数
@param completion 预测完成回调
*/
- (void)predictWithBuffer:(id<MTLBuffer>)buffer channelNum:(NSInteger)channelNum withErrorResultCompletion:(void (^)(NSError * _Nullable, NSArray <PaddleGPUResult *> *))completion;


/*
 * 清理内存
 */
-(void)clear;

@end

NS_ASSUME_NONNULL_END

