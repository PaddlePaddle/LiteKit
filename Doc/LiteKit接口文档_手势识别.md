# 手势识别接口文档
手势识别能力由LiteKit HandGestureDetection SDK提供的，实时的手势检测能力。可以准确的检测手势所在位置的矩形坐标、手势类型、置信度。

### 目录
<a href="#iOSAPI">iOS API</a>
<br>
<a href="#AndroidAPI">Android API</a>
<br>


## 检测内容
- 手势类型
- 手势位置的矩形坐标
- 置信度

| <B>手势</B> |  手  | 五指收拾 | 胜利V手势 | 握拳收拾 | 指/1手势 |  OK手势 | 
| --- | --- |--- |--- |--- |--- |--- |
| <B>图例</B> |  ![图片](/Doc/Resources/4_1.png) |  ![图片](/Doc/Resources/4_2.png) | ![图片](/Doc/Resources/4_3.png) |![图片](/Doc/Resources/4_4.png)  |![图片](/Doc/Resources/4_5.png) | ![图片](/Doc/Resources/4_6.png) | 


<div id="iOSAPI"></div>

## iOS API

### 1. 创建实例
手势识别通过createGestureDetector创建detector之后，可以通过UIImage、CMSampleBufferRef、uint8_t * 三种数据作为input执行预测。

#### 参数
- error：创建过程中如果发生错误，返回错误信息。如果成功创建，不会赋值。
#### 返回值
- 创建成功的手势识别Detector

```objective-c
LiteKitHandGestureDetector.h

/**
 * @brief initialize and return a instance of hand gesture detector, the model should be put in main bundle
 *
 * @param error if an error occurs, this param will carry the information
 * @return a instance of gesture detector
 */
+ (instancetype)createHandGestureDetectorWithError:(NSError **)error;
```


### 2. 执行预测

1）通过数据进行预测

#### 参数
- pixelRawData：待预测图像数据，uint8_t 4 channel的RGBA图像数据
- width：待预测图像数据的宽
- height：待预测图像数据的高
- complete：预测结束回调，result为预测的结果，error为预测过程中出现的错误。</br>
如果error!=nil，表示预测出现错误，result为空。如果error==nil，表示预测成功，result为预测结果。</br>
LiteKitHandGestureDetectResult结果说明见[**4.预测结果数据模型**](#LiteKitHandGestureDetectResult)。

```objective-c
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
                      complete:(void (^)(LiteKitHandGestureDetectResult *result, NSError *error))complete;

@end
```


2）通过图片进行预测
其他参数同1），其中输入图片数据为UIImage格式。
```objective-c
/**
 * @brief detect hand gesture with image
 *
 * @param image UIImage that will be detected
 * @param complete completion block with result and error imformation
 */
- (void)detectWithUIImage:(UIImage *)image
                 complete:(void (^)(LiteKitHandGestureDetectResult *result, NSError *error))complete;
```

3）通过CMSampleBufferRef进行预测
其他参数同1），其中输入图片数据为CMSampleBufferRef格式。
```objective-c
/**
 * @brief detect hand gesture with sampleBuffer
 *
 * @param sampleBuffer sampleBuffer that will be detected
 * @param complete completion block with result and error imformation
 */
- (void)detectWithSampleBuffer:(CMSampleBufferRef)sampleBuffer
                      complete:(void (^)(LiteKitHandGestureDetectResult *result, NSError *error))complete;
```


### 3. 释放
iOS的手势识别接口ARC下不需要额外的释放操作


### 4. 预测结果数据模型
#### LiteKitHandGestureDetectResult
手势识别结果数据结构的定义
```objective-c
/**
 * define hand gesture detect result data
 */
@interface LiteKitHandGestureDetectResult : NSObject
```
LiteKitHandGestureDetectResult包含一下属性：
手所在位置的矩形坐标。
```objective-c
// rect of hand‘s bounding box
@property (nonatomic, assign, readonly) CGRect handBoxRect;
```

手指尖所在点的坐标。
```objective-c
// position of finger
@property (nonatomic, assign, readonly) CGPoint fingerPoint;
```

手势类型的枚举，标志可识别手势类型的枚举的定义，目前支持6种。
| 手势| 说明 | 
| --- | --- | 
| LiteKitHandGestureDetectionLabelHand = 0 | 手 |
| LiteKitHandGestureDetectionLabelFive = 1 | 五指手势 |
| LiteKitHandGestureDetectionLabelVictory = 2 | 胜利V手势 |
| LiteKitHandGestureDetectionLabelFist = 3 | 握拳手势 |
| LiteKitHandGestureDetectionLabelOne = 4 | 指/1手势 |
| LiteKitHandGestureDetectionLabelOK = 5 | OK手势 |
```objective-c
// label of detection result
@property (nonatomic, assign, readonly) LiteKitHandGestureDetectionLabel label;
```

置信度，值位于0～1之间。
```objective-c
// confidence of detection result
@property (nonatomic, assign, readonly) float confidence;

```

### 5. Demo Code
```objective-c
// 引入头文件
#import <LiteKitHandGestureDetection/LiteKitHandGestureDetector.h>

// 创建Predictor
LiteKitHandGestureDetector *gestureRecognizer = [LiteKitHandGestureDetector createHandGestureDetectorWithError:&error];

// 执行Predict、获取Output
[gestureRecognizer detectWithUIImage:self.image complete:^(LiteKitHandGestureDetectResult *result, NSError *error) {
    //result为获取的output
}];

// 释放Predictor
// Predictor不需要特殊的释放操作
```


<div id="AndroidAPI"></div>

## Android API
### 1. 创建实例
对手势识别的HandGestureDetector进行初始化
```java

/**
 * @desc 对HandGestureDetector进行初始化
 * @param context 上下文contexts
 */
public static boolean init(Context context)
```

### 2. 执行预测
通过原始数据进行预测的接口
#### 参数
- data 图像的数据
- imgWidth 图像的宽度
- imgHeight 图像的高度

#### 返回值
HandGestureDetectResult 说明见[**4. 预测结果数据模型**](#HandGestureDetectResult)
```java

/**
 * @desc 通过byte数据进行预测
 * @param data 待预测的数据
 * @param imgWidth 图像Width
 * @param imgHeight 图像Height
 * @return HandGestureDetectResult 预测结果
 */
public static HandGestureDetectResult detect(byte[] data, int imgWidth, int imgHeight) 
```

通过Bitmap进行预测的接口，其中接口通过Bitmap接受图像数据，返回值意义同1）通过原始数据预测接口
```java

/**
 * @desc 通过bitmap进行预测
 * @param image 待预测的内容的bitmap
 * @return HandGestureDetectResult 预测结果
 */
public static HandGestureDetectResult detect(Bitmap image);
```
### 3. 释放
使用完成之后，对手势识别进行释放
```java

/**
 * @desc 对HandGestureDetector中需要释放的资源进行释放
 */
public static boolean release()
```
### 4. 预测结果数据模型
#### HandGestureDetectResult
预测返回结果结构HandGestureDetectResult
```java
public class HandGestureDetectResult {
    public RectF handBoxRect; // 识别出的手势的位置的的rect
    public PointF fingerPoint; // 手势中手指的位置的point
    public HandGestureDetectResult.GestureLabel label; // 识别出手势的类型
    public float confidence; // 置信度

    public static enum GestureLabel {
        Hand, //手
        Five, // 五指手势
        Victory, // 胜利V手势
        Fist, // 握拳手势
        One, // 指/1手势
        OK; // OK手势

        private GestureLabel() {
        }
    }
}
```

### 5. Demo Code
```java

// 引入头文件
import com.baidu.litekit.HandGestureDetectResult;
import com.baidu.litekit.HandGestureDetector;

// 创建Predictor
HandGestureDetector.init(this);

// 执行Predict、获取Output
HandGestureDetectResult result = HandGestureDetector.detect(imageData);

// 释放Predictor
HandGestureDetector.release();
```

