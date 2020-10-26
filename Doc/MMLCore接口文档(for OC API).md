# MMLCore接口文档(for OC API)
## MMLBaseMachine
Machine的基类 
具体实现包含MMLPaddleCPUMachine、MMLPaddleGPUMachine

```
#pragma mrak - Predict
/// sync
/// @param aInputData MMLData类型的输入数据
/// @param aError 错误信息
- (MMLData * __nullable)predictWithInputData:(MMLData * __nonnull)aInputData error:(NSError **)aError;

/// async
/// @param aInputData MMLData类型的输入数据
/// @param aBlock 完成回调
- (void)predictWithInputData:(MMLData * __nonnull)aInputData completionBlock:(MMLMachinePredictCompletionBlock)aBlock;

/// 运行带performance信息回调的machine
/// @param aInputData input数据
/// @param aBlock 带performance信息的回调
- (void)predictWithInputData:(MMLData * __nonnull)aInputData performanceBlock:(MMLMachinePredictPerformanceBlock)aBlock;

#pragma mark - Clear

/// 清理machine内存，不释放Machine
- (void)clearMachine;

#pragma mark - Release

/// 释放Machine,置空Machine
- (void)releaseMachine;


```

## MMLInferenceEngineConfigProtocol

MMLMachineService通过加载config创建basemachine
```
/// Machine的类型
typedef NS_ENUM (NSUInteger, MMLMachineType) {
    MMLPaddleGPU,              // paddle GPU Machine
    MMLPaddleCPU               // paddle CPU Machine
};

/// MMLMachine的公共协议
@protocol MMLMachineConfigProtocol <NSObject>
/// Machine类型,参数必传，代表使用什么底层inference框架
@property (nonatomic, assign) MMLMachineType machineType;
/// 模型文件地址，通过已存在的Model地址加载Machine
@property (nonatomic, copy) NSString *modelPath;
/// inference引擎特殊的配置
@property (nonatomic, strong) id <MMLInferenceEngineConfigProtocol> engineConifg;
/// 可以设置自定义的logger
@property (nonatomic, copy) NSString *loggerClassName;
```

## MMLMachineService
根据config的配置创建对应的machine
```

/// service load阶段错误值枚举
typedef NS_ENUM(NSInteger,MMLMachineServiceLoadErrorCode) {
    MMLMachineServiceLoadMachineTypeError = 0,      // 错误的machine类型
    MMLMachineServiceLoadNotSupportSimulator,       // 不支持模拟器
    MMLMachineServiceLoadNotSupportArchitecture,    // 不支持的处理器架构
    MMLMachineServiceLoadWrongConfig,               // 错误的配置
    MMLMachineServiceLoadNoModelFile,               // 没有模型文件
    MMLMachineServiceLoadNoModelPointer,            // 内存指针为空
};


/// @param machine 加载成功的machine
/// @param error error信息
typedef void (^MMLMachineLoadCompletionBlock)(MMLMachine * __nullable machine, NSError * __nullable error);


/// Machine Service类，负责Machine的创建、运行以及释放
@interface MMLMachineService : NSObject

/// 当前对应的machine
@property (nonatomic, strong, readonly) MMLBaseMachine * __nullable mmlMachine;
/// Service的唯一标识
@property (nonatomic, copy, readonly) NSString *machineServiceId;
/// 性能统计数据
@property (nonatomic, strong, readonly) MMLPerformanceProfiler *performanceDataMap;

#pragma mark - Load

/// sync 通过配置加载Machine
/// @param aConfig MML配置
/// @param aError 错误信息
- (MMLBaseMachine * __nullable)loadMachineWithConfig:(MMLMachineConfig * __nonnull)aConfig error:(NSError **)aError;

/// async
/// @param aConfig MML配置
/// @param aBlock 完成回调
- (void)loadMachineWithConfig:(MMLMachineConfig * __nonnull)aConfig completionBlock:(MMLMachineLoadCompletionBlock)aBlock;


```


## MMLDataProcess
input& output的处理工具集，可以辅助进行前后处理

```

/**
  上采样
 
  @param src 待采样mat
  @param samplingRate 采样比例
  @return sampling后的Mat数据，如果samplingRate是0，则为非法，会返回empty的mat
 */
+ (Mat)mml_subSamplingCVMat:(Mat)src rate:(double)samplingRate;
/**
 下采样

 @param src 待采样mat
 @param samplingRate 采样比例
 @return sampling后的Mat数据
*/
+ (Mat)mml_upSamplingCVMat:(Mat)src rate:(double)samplingRate;

/**
 UIimage -> Mat互转方法

 @param src 入参数图片数据
 @return 转换后的UIimage数据
*/
+ (UIImage *)mml_UIImageFromCVMat:(Mat)src;

/**
 Mat -> UIimage转换

 @param src 入参数图片数据
 @return 转换后的Mat数据
*/
+ (Mat)mml_CVMatFromUIImage:(UIImage *)src;


/**
 normalization方法

 @param src 入参数图片数据
 @param outputType 返回Mat数据类型
 @param alpha 转换系数，return = src * alpha + beta
 @param beta 转换偏移，return = src * alpha + beta
 @return Mat 返回转换后的数据。如果outputType是无法支持的类型，会返回empty的mat
*/
+ (Mat)mml_CVMatNormalization:(Mat)src
                   outputType:(TMMLMatDataType)outputType
                        alpha:(double)alpha
                         beta:(double)beta;

/**
 Mat Resize方法

 @param src 入参数图片数据
 @param size output的size
 @return Mat scale后的size
*/
+ (Mat)mml_scaleCVMat:(Mat)src toSize:(Size2i)size;

/**
 RGB的MML图片数据结构转Mat

 @param ycrcbData YCrCb数据
 @return Mat 转换后的Mat，如果ycrcbData通道数不是1、3、4则无法支持，会返回empty的mat
*/
+ (Mat)mml_imageRGBCVMatFromYCrCb:(MMLImageData)ycrcbData;

/**
 RGB的MML图片数据结构转Mat

 @param yuvData YUV数据
 @return Mat 转换后的Mat，如果ycrcbData通道数不是1、3、4则无法支持，会返回empty的mat
*/
+ (Mat)mml_imageRGBCVMatFromYUV:(MMLImageData)yuvData;

/**
 Mat转MML图片数据

 @param imageMat YCrCb数据
 @return MMLImageData RGB的MML图片数据结构，如果imageMat通道数不是1、3、4则无法支持，会返回empty的mat
*/
+ (MMLImageData)mml_YCrCbFromImageRGBCVMat:(Mat)imageMat;

/**
 Mat转MML图片数据

 @param imageMat YUV数据
 @return MMLImageData RGB的MML图片数据结构，如果imageMat通道数不是1、3、4则无法支持，会返回empty的mat
*/
+ (MMLImageData)mml_YUVFromImageRGBCVMat:(Mat)imageMat;

/**
 Mat -> float *转换

 @param inputMat Mat数据
 @param image_data float*数据,待填充
*/
+ (void)mml_convertCVMatData:(Mat)inputMat toFloatData:(float *)image_data;///Mat    >> float*

/**
 float * -> Mat转换

 @param image_data float*数据
 @param inputMat Mat数据，待填充
*/
+ (void)mml_convertFloatData:(float *)image_data toCVMatData:(Mat)inputMat;///float* >> Mat

/**
 Mat -> uint8_t *转换

 @param inputMat Mat数据
 @param image_data uint8_t*数据,待填充
*/
+ (void)mml_convertCVMatData:(Mat)inputMat toUint8Data:(uint8_t *)image_data;

/**
 uint8_t * -> Mat转换
 
 @param image_data uint8_t*数据
 @param inputMat Mat数据，待填充
*/
+ (void)mml_convertUint8Data:(uint8_t *)image_data toCVMatData:(Mat)inputMat;

/**
 剪裁Mat

 @param inputMat Mat数据
 @param rect 剪裁的区域
 @return Mat 剪裁后的Mat
*/
+ (Mat)mml_shearCVMat:(Mat)inputMat rect:(Rect2i)rect;

/**
 填充Mat

 @param inputMat Mat数据
 @param margin 填充的margin
 @param borderTypre 填充的方法
 @return Mat 剪裁后的Mat
*/
+ (Mat)mml_borderCVMat:(Mat)inputMat margin:(MMLMargin)margin type:(TMMLBorderTypes)borderTypre;


```

## MMLData
input & output 数据结构定义

1. 枚举定义
```

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
```
封装的MML数据类型
```
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

```
MMLData的定义
```

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
```
