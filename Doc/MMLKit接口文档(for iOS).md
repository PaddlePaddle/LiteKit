# MMLKit接口文档(for iOS)

## 人像分割
人像分割能力的output数据结构定义
```
/**
 * @desc PortraitSegmentor data for output
 */
typedef struct MMLPortraitSegmentorData {
    struct MMLPortraitSegmentorDataShape {
        int n; // batch, =1
        int c; // channel, =1
        int h; // output height, =192
        int w; // output width, =192
    } dataShape; // output data shape
    uint8_t *data; // output data
} MMLPSData;

```
人像分割Segmentor的创建和执行，通过create方法创建Segmentor之后，可以以uint8_t、UIImage和CVPixelBufferRef三种作为input数据执行预测。
```
/**
 * @desc MML Portrait Segmentor Implement
 */
@interface MMLPortraitSegmentor : NSObject

#pragma mark - MMLPortraitSegmentor/Create

/**
 * @desc create Portrait Segmentor
 * @param modelPath Path for Portrait Segmentor model
 * @param error error while create, if succeed will be nil, nullable
 * @return instancetype Portrait Segmentor created
 */
+ (instancetype) create:(NSString *)modelPath
                  error:(NSError **)error;


#pragma mark - MMLPortraitSegmentor/Infer-Sync

/**
 * @desc Portrait Segmentor inference with RawData
 * @param rawData rawData to inference
 * @param width input Data width
 * @param height input data height
 * @param error  Error while inference, will be nil if succed
 * @return MMLPSData output data, size of 192*192
 */
- (MMLPSData *) inferWithRawData:(uint8_t *)rawData
                           width:(int)width
                          height:(int)height
                           error:(NSError **)error;

/**
 * @desc Portrait Segmentor inference with UIImage
 * @param image UIImage to inference
 * @param error  Error while inference, will be nil if succed
 * @return MMLPSData output data, size of 192*192
 */
- (MMLPSData *) inferWithImage:(UIImage *)image
                         error:(NSError **)error;

/**
 * @desc Portrait Segmentor inference with CVPixelBufferRef
 * @param pixelBuffer CVPixelBufferRef to inference
 * @param error  Error while inference, will be nil if succed
 * @return MMLPSData output data, size of 192*192
 */
- (MMLPSData *) inferWithPixelBuffer:(CVPixelBufferRef)pixelBuffer
                               error:(NSError **)error;

@end
```

## 手势识别
手势识别label枚举的定义，目前支持6种识别label
```
/**
 * define hand gesture detection result label
 */
typedef NS_OPTIONS(NSUInteger, MMLHandGestureDetectionLabel) {
    MMLHandGestureDetectionLabelHand      = 0,
    MMLHandGestureDetectionLabelFive      = 1,
    MMLHandGestureDetectionLabelVictory   = 2,
    MMLHandGestureDetectionLabelFist      = 3,
    MMLHandGestureDetectionLabelOne       = 4,
    MMLHandGestureDetectionLabelOK        = 5,
};
```
手势识别结果数据结构的定义
```
/**
 * define hand gesture detect result data
 */
@interface MMLHandGestureDetectResult : NSObject

// rect of hand‘s bounding box
@property (nonatomic, assign, readonly) CGRect handBoxRect;

// position of finger
@property (nonatomic, assign, readonly) CGPoint fingerPoint;

// label of detection result
@property (nonatomic, assign, readonly) MMLHandGestureDetectionLabel label;

// confidence of detection result
@property (nonatomic, assign, readonly) float confidence;

@end
```
手势识别detector的API，通过createGestureDetector创建detector之后，可以通过UIImage、CMSampleBufferRef、uint8_t * 三种数据作为input执行预测。
```
/**
 * hand gesture detector
 */
@interface MMLHandGestureDetector : NSObject

/**
 * @brief initialize and return a instance of gesture detector, the model should be put in main bundle
 *
 * @param error if an error occurs, this param will carry the information
 * @return a instance of gesture detector
 */
+ (instancetype)createGestureDetector:(NSError **)error;

/**
 * @brief initialize and return a instance of gesture detector with model path
 *
 * @param path the folder where the model is located
 * @param error if an error occurs, this param will carry the information
 * @return a instance of gesture detector
 */
+ (instancetype)createGestureDetectorWithModelPath:(NSString *)path error:(NSError **)error;

/**
 * @brief detect hand gesture with image
 *
 * @param image UIImage that will be detected
 * @param complete completion block with result and error imformation
 */
- (void)detectWithUIImage:(UIImage *)image
                 complete:(void (^)(MMLHandGestureDetectResult *result, NSError *error))complete;

/**
 * @brief detect hand gesture with sampleBuffer
 *
 * @param sampleBuffer sampleBuffer that will be detected
 * @param complete completion block with result and error imformation
 */
- (void)detectWithSampleBuffer:(CMSampleBufferRef)sampleBuffer
                      complete:(void (^)(MMLHandGestureDetectResult *result, NSError *error))complete;

/**
 * @brief detect hand gesture with pixelRawData
 *
 * @param pixelRawData pixelRawData that will be detected, pixel format shoud be RGBA
 * @param width detect buffer width
 * @param height detect buffer height
 * @param complete completion block with result and error imformation
 */
- (void)detectWithPixelRawData:(uint8_t *)pixelRawData
                         width:(int)width
                        height:(int)height
                      complete:(void (^)(MMLHandGestureDetectResult *result, NSError *error))complete;

@end
```

## 视频超分
视频超分使用的是YUV数据，以下为YUV数据结构定义，用于预测的input和output
```
///video frame YUV420 data struct
typedef struct {
    // frame width
    int width;
    // frame height
    int height;
    // data light (include Padding)
    int y_stride;
    int u_stride;
    int v_stride;
    //YUVdata
    uint8_t* y_data;
    uint8_t* u_data;
    uint8_t* v_data;
} MMLYUV420Data;//video 420 frame data

/**
 *release MMLYUVData
 * @param data input数据
 */
void releaseYUV420Data(MMLYUV420Data *data);

/**
 * copy MMLYUVData
 * @param src input data
 * @param dst output data
 */
void copyYUV420Data(const MMLYUV420Data *src, MMLYUV420Data *dst);

```
视频超分的config定义，需要在创建超分inferencer的时候，传入config。
config需要配置模型在工程中的位置。
```

/// video super resolution config
@interface MMLVideoSuperResolutionConfig : NSObject
/// model file dir
@property (nonatomic, strong) NSString *modelDir;

@end

```

视频超分能力inferencer，通过createInstanceWithConfig创建完成inferencer之后，可以通过
UIImage和MMLYUV420Data两种数据作为input执行超分。
```
/**
* creat Video superresolution inferencer
*/
@interface MMLVideoSuperResolutionor : NSObject

/**
 * @brief initialize and return a instance of video super resolutionor
 *
 * @param config config for superresolution
 * @param error if an error occurs, this param will carry the information
 * @return a instance of gesture detector
 */
+ (instancetype)createInstanceWithConfig:(MMLVideoSuperResolutionConfig *)config
                                   error:(NSError **)error;


/**
 * @brief execute videoSuperSolution
 *
 * @param inputData input data, struct MMLYUVData
 * @param outputData output data
 * @param error errorcode and error reason
 * @return BOOL YES for succeed，NO for failure
 */
- (BOOL)superResolutionWithInputData:(const MMLYUV420Data *)inputData
                          outputData:(MMLYUV420Data *)outputData
                               error:(NSError **)error;


/**
 * @brief execute videoSuperSolution
 *
 * @param inputImage input image, UIImage
 * @param error errorcode and error reason
 * @return UIImage image after superresolution
 */
- (UIImage *)superResolutionWithUIImage:(UIImage *)inputImage
                                  scale:(CGFloat)scale
                                  error:(NSError **)error;

@end
```
