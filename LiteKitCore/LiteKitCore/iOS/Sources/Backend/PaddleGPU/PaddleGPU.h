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


#import <Metal/Metal.h>
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

typedef enum : NSUInteger {
    SuperResolutionNetType, // SuperResolution Net
    MobileNetCombileType,   // mobile Net
    YoloNetType,            // Yolo  Net
    CustomNetType,          // custom Net, Net Params from ModelConfig
} NetType;

typedef enum : NSUInteger {
    PrecisionTypeFloat32 = 1,
    PrecisionTypeFloat16 = 2
} PrecisionType;

typedef enum : NSUInteger {
    LoadMetalInPaddleGPU   = 1,        // use metal in paddleGPU
    LoadMetalInDefaultLib     = 2,     // use metal in main bundle
    LoadMetalInCustomMetalLib = 3      // use metal framework
} MetalLoadType;

@interface PaddleGPUResult: NSObject

@property (assign, nonatomic) float *output;

@property (assign, nonatomic) int outputSize;

@property (strong, nonatomic) NSArray <NSNumber *>*dim;

-(void)releaseOutput;

@end

@interface PaddleGPUConfig: NSObject


/*
 * dims info  [n c h w]
 */
@property (strong, nonatomic) NSArray<NSNumber *> *dims;


/*
 * pointer to params
 */
@property (assign, nonatomic) void *paramPointer;

/*
 * params sizes (kb)
 */
@property (assign, nonatomic) int paramSize;

/*
 * pointer to model
 */
@property (assign, nonatomic) void *modelPointer;

/*
 * model size (kb)
 */
@property (assign, nonatomic) int modelSize;

/**
 whether use MetalPerformanceShaders to calculate, when precision is 32 should not enable this option, default is false
 */
@property (assign, nonatomic) BOOL useMPS;

/**
 Aggressive Optimization Strategy, may harm to precision, default is false
 */
@property (assign, nonatomic) BOOL useAggressiveOptimization;

/**
 model precision
 */
@property (assign, nonatomic) PrecisionType modelPrecision;

/**
 metallib load type, when metalLoadType is LoadMetalInCustomMetalLib ， metal library should not be empty
 */
@property (assign, nonatomic) MetalLoadType metalLoadType;

/**
 metal lib path
 */
@property (copy, nonatomic) NSString *metalLibPath;


/**
 calculate precision， Immutable in a runner lifecycle
 */
@property (assign, nonatomic) PrecisionType computePrecision;

/**
 debug switch
 */
@property (assign, nonatomic) BOOL enableDebug;


@end

@interface PaddleGPU: NSObject

/**
 init
 */
-(instancetype)initWithCommandQueue:(id<MTLCommandQueue>)queue net:(NetType)netType modelConfig:(PaddleGPUConfig *)config;


/**
 load runner

 @return load result
 */
-(BOOL)load;


/**
 load runner
 
 @param error error message
 */
- (void)loadWithError:(NSError *__autoreleasing  _Nullable * _Nullable)error;



/**
 update dim
 
 @param dims dims to updatet
 */
- (BOOL)updateInputDim:(NSArray <NSNumber *>*)dims ;


/**
 update dim
 
 @param dims dims to updatet
 @param error error message
 */
- (void)updateInputDimInDim:(NSArray <NSNumber *>*)dims error:(NSError *__autoreleasing  _Nullable * _Nullable)error;


/**
 get texture from imageRef
 
 @param image imageref
 @param block texture callback
 */
- (void)getTextureWithImage:(CGImageRef)image getTexture:(void (^) (id<MTLTexture>))block;

/**
 get texture from MTLBuffer，channel=1
 
 @param buffer MTLBuffer
 @param block texture callback
 */
- (void)getTextureWithBuffer:(id<MTLBuffer>)buffer getTexture:(void (^) (id<MTLTexture>))block;

/**
 get texture from MTLBuffer
 
 @param buffer MTLBuffer
 @param block texture callback
 @param channelNum channel number
 */
- (void)getTextureWithBuffer:(id<MTLBuffer>)buffer getTexture:(void (^) (id<MTLTexture>))block channelNum:(NSInteger)channelNum;

/**
 predict with complete callback
 
 @param texture texture from image need to be predict
 @param completion  predict complete callback
 */
-(void)predict:(id<MTLTexture>)texture withCompletion:(void (^)(BOOL, NSArray<NSArray <NSNumber *>*> *))completion;

/**
 predict with error message，output is origin data
 
 @param texture texture from image need to be predict
 @param completion predict complete callback
 */
-(void)predict:(id<MTLTexture>)texture withErrorCompletion:(void (^)(NSError * _Nullable, NSArray<NSArray <NSNumber *>*> *))completion;


/**
 predict with completion callback, output format as PaddleGPUResult
 * @param texture texture from image need to be predict
 * @param completion predict complete callback
 */
-(void)predict:(id<MTLTexture>)texture withResultCompletion:(void (^)(BOOL, NSArray <PaddleGPUResult *> *))completion;

/**
 predict with completion callback, output format as PaddleGPUResult
 * @param texture texture from image need to be predict
 * @param completion predict complete callback
 */
-(void)predict:(id<MTLTexture>)texture withErrorResultCompletion:(void (^)(NSError * _Nullable, NSArray <PaddleGPUResult *> *))completion;

/**
 predict with buffer, output format as PaddleGPUResult
 * @param buffer buffer in memory
 * @param channelNum channel number
 * @param completion predict complete callback
 */
- (void)predictWithBuffer:(id<MTLBuffer>)buffer channelNum:(NSInteger)channelNum withResultCompletion:(void (^)(BOOL, NSArray <PaddleGPUResult *> *))completion;

/**
 predict with buffer, output format as PaddleGPUResult
 * @param buffer buffer in memory
 * @param channelNum channel number
 * @param completion predict complete callback
 */
- (void)predictWithBuffer:(id<MTLBuffer>)buffer channelNum:(NSInteger)channelNum withErrorResultCompletion:(void (^)(NSError * _Nullable, NSArray <PaddleGPUResult *> *))completion;


/**
 * clean memory
 */
-(void)clear;

@end

NS_ASSUME_NONNULL_END

