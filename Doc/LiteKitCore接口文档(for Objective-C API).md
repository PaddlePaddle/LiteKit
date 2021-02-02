# LiteKitCore接口文档(for Objective-C API)
## LiteKitBaseMachine
Machine的基类 
具体实现包含LiteKitPaddleCPUMachine、LiteKitPaddleGPUMachine

```objectivec

#pragma mrak - Predict
/// sync
/// @param aInputData LiteKitData input data
/// @param aError error message
- (LiteKitData * __nullable)predictWithInputData:(LiteKitData * __nonnull)aInputData error:(NSError **)aError;

/// async
/// @param aInputData LiteKitData input data
/// @param aBlock complete block
- (void)predictWithInputData:(LiteKitData * __nonnull)aInputData completionBlock:(LiteKitMachinePredictCompletionBlock)aBlock;

/// predict with performance message
/// @param aInputData input data
/// @param aBlock callback with performance
- (void)predictWithInputData:(LiteKitData * __nonnull)aInputData performanceBlock:(LiteKitMachinePredictPerformanceBlock)aBlock;

#pragma mark - Clear

/// clear machine memory，not release Machine
- (void)clearMachine;

#pragma mark - Release

/// release Machine,set Machine null
- (void)releaseMachine;

```

## LiteKitMachineService
根据config的配置创建对应的machine
```objectivec

/// service load period error enum
typedef NS_ENUM(NSInteger,LiteKitMachineServiceLoadErrorCode) {
    LiteKitMachineServiceLoadMachineTypeError = 0,      // error machine type
    LiteKitMachineServiceLoadNotSupportSimulator,       // Simulator not support
    LiteKitMachineServiceLoadNotSupportArchitecture,    // Architecture not support
    LiteKitMachineServiceLoadWrongConfig,               // error config
    LiteKitMachineServiceLoadNoModelFile,               // model file not found
    LiteKitMachineServiceLoadNoModelPointer,            // model pointer is null
};


/// @param machine succeed loaded machine
/// @param error error message
typedef void (^LiteKitMachineLoadCompletionBlock)(LiteKitMachine * __nullable machine, NSError * __nullable error);


/// Machine Service class，charge of Machine load、run and release
@interface LiteKitMachineService : NSObject

/// current machine
@property (nonatomic, strong, readonly) LiteKitBaseMachine * __nullable litekitMachine;
/// Service identifier
@property (nonatomic, copy, readonly) NSString *machineServiceId;
/// performance data
@property (nonatomic, strong, readonly) LiteKitPerformanceProfiler *performanceDataMap;

#pragma mark - Load

/// sync load Machine with conofig
/// @param aConfig LiteKit config
/// @param aError error message
- (LiteKitBaseMachine * __nullable)loadMachineWithConfig:(LiteKitMachineConfig * __nonnull)aConfig error:(NSError **)aError;

/// async
/// @param aConfig LiteKit config
/// @param aBlock complete callback
- (void)loadMachineWithConfig:(LiteKitMachineConfig * __nonnull)aConfig completionBlock:(LiteKitMachineLoadCompletionBlock)aBlock;


```


## LiteKitDataProcess
input& output的处理工具集，可以辅助进行前后处理

```objectivec

/**
  up sampling
 
  @param src input mat
  @param samplingRate sampling rate
  @return Mat after sampling，if samplingRate=0，invalid，will return empty mat
 */
+ (Mat)litekit_subSamplingCVMat:(Mat)src rate:(double)samplingRate;
/**
 down sampling

 @param src inputmat
 @param samplingRate sampling rate
 @return Mat after sampling
*/
+ (Mat)litekit_upSamplingCVMat:(Mat)src rate:(double)samplingRate;

/**
 UIimage -> Mat convert

 @param src input image data
 @return converted UIimage data
*/
+ (UIImage *)litekit_UIImageFromCVMat:(Mat)src;

/**
 Mat -> UIimage convert

 @param src input image data
 @return converted Mat data
*/
+ (Mat)litekit_CVMatFromUIImage:(UIImage *)src;


/**
 normalization

 @param src input image data
 @param outputType retturn Mat data type
 @param alpha coefficient，return = src * alpha + beta
 @param beta shifting，return = src * alpha + beta
 @return Mat converted data。if outputType not support，will return empty mat
*/
+ (Mat)litekit_CVMatNormalization:(Mat)src
                   outputType:(TLiteKitMatDataType)outputType
                        alpha:(double)alpha
                         beta:(double)beta;

/**
 Mat Resize

 @param src  input image data
 @param size output size
 @return Mat scaled size
*/
+ (Mat)litekit_scaleCVMat:(Mat)src toSize:(Size2i)size;

/**
 RGB LiteKit image data struct convert to Mat

 @param ycrcbData YCrCb data
 @return Mat converted mat，if ycrcbData channels != 1、3、4 will not support，will return empty mat
*/
+ (Mat)litekit_imageRGBCVMatFromYCrCb:(LiteKitImageData)ycrcbData;

/**
 RGB LiteKit image data struct convert to Mat

 @param yuvData YUV data
 @return Mat converted mat，if ycrcbData channels != 1、3、4 will not suppoort，will return empty mat
*/
+ (Mat)litekit_imageRGBCVMatFromYUV:(LiteKitImageData)yuvData;

/**
 Mat to LiteKit image data

 @param imageMat YCrCb data
 @return LiteKitImageData RGB LiteKit image data struct，if imageMat channels != 1、3、4will not support，will return empty mat
*/
+ (LiteKitImageData)litekit_YCrCbFromImageRGBCVMat:(Mat)imageMat;

/**
 Mat转LiteKit image data struct

 @param imageMat YUV data
 @return LiteKitImageData RGB LiteKit image data struct，if imageMat channels != 1、3、4will not support，will return empty mat
*/
+ (LiteKitImageData)litekit_YUVFromImageRGBCVMat:(Mat)imageMat;

/**
 Mat -> float * converter

 @param inputMat Mat data
 @param image_data float* data, to be writed
*/
+ (void)litekit_convertCVMatData:(Mat)inputMat toFloatData:(float *)image_data;///Mat    >> float*

/**
 float * -> Mat converter

 @param image_data float* data
 @param inputMat Mat data，to be writed
*/
+ (void)litekit_convertFloatData:(float *)image_data toCVMatData:(Mat)inputMat;///float* >> Mat

/**
 Mat -> uint8_t * converter

 @param inputMat Mat data
 @param image_data uint8_t* data,to be writed
*/
+ (void)litekit_convertCVMatData:(Mat)inputMat toUint8Data:(uint8_t *)image_data;

/**
 uint8_t * -> Mat converter
 
 @param image_data uint8_t* data
 @param inputMat Mat data，to be writed
*/
+ (void)litekit_convertUint8Data:(uint8_t *)image_data toCVMatData:(Mat)inputMat;

/**
 shear Mat

 @param inputMat Mat data
 @param rect shear rect
 @return Mat sheared Mat
*/
+ (Mat)litekit_shearCVMat:(Mat)inputMat rect:(Rect2i)rect;

/**
 border Mat

 @param inputMat Mat data
 @param margin border margin
 @param borderTypre border type
 @return Mat border Mat
*/
+ (Mat)litekit_borderCVMat:(Mat)inputMat margin:(LiteKitMargin)margin type:( TLiteKitBorderTypes)borderTypre;


```

## LiteKitData
input & output 数据结构定义

1. 枚举定义
```objectivec

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

```
封装的LiteKit数据类型
```objectivec

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

```
LiteKitData的定义
```objectivec

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
```
