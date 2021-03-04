# 人像分割接口文档
人像分割能力由LiteKit PortraitSegmentation SDK提供的，对图像中人像部分进行分割的能力。

### 目录
<a href="#iOSAPI">iOS API</a>
<br>
<a href="#AndroidAPI">Android API</a>
<br>

<div id="iOSAPI"></div>

## iOS API

### 1. 创建实例
人像分割Segmentor的创建和执行，通过create方法创建Segmentor之后，可以以uint8_t、UIImage和CVPixelBufferRef三种作为input数据执行预测。

#### 参数
- error：创建过程中如果发生错误，返回错误信息。如果成功创建，不会赋值。
#### 返回值
- 创建成功的人像分割的Segmentor

```objective-c
LiteKitPortraitSegmentor.h

/**
 * @desc create Portrait Segmentor
 * @param error error while create, if succeed will be nil, nullable
 * @return instancetype Portrait Segmentor created
 */
+ (instancetype) createPortraitSegmentorWithError:(NSError **)error;
```

### 2. 执行预测
1）通过数据进行预测

#### 参数
- rawData：待预测图像数据，uint8_t 3 channel的RGB图像数据
- width：待预测图像数据的宽
- height：待预测图像数据的高
- error：error为预测过程中出现的错误，如果error!=nil，表示预测出现错误，result为空。如果error==nil，表示预测成功，result为预测结果。</br>

#### 返回值
人像分割的预测结果，LiteKitPSData说明见[**4.预测结果数据模型**](#LiteKitPSData)。
```objective-c

/**
 * @desc Portrait Segmentor inference with RawData
 * @param rawData rawData to inference
 * @param width input Data width
 * @param height input data height
 * @param error  Error while inference, will be nil if succed
 * @return LiteKitPSData output data, size of 192*192
 */
- (LiteKitPSData *) inferWithRawData:(uint8_t *)rawData
                           width:(int)width
                          height:(int)height
                           error:(NSError **)error;
```

2）通过UIImage进行预测
其他参数同1），其中输入图片数据为UIImage格式。
```objective-c
/**
 * @desc Portrait Segmentor inference with UIImage
 * @param image UIImage to inference
 * @param error  Error while inference, will be nil if succed
 * @return LiteKitPSData output data, size of 192*192
 */
- (LiteKitPSData *) inferWithImage:(UIImage *)image
                         error:(NSError **)error;
```

3）通过CVPixelBufferRef进行预测
其他参数同1），其中输入图片数据为UIImage格式。
```objective-c
/**
 * @desc Portrait Segmentor inference with CVPixelBufferRef
 * @param pixelBuffer CVPixelBufferRef to inference
 * @param error  Error while inference, will be nil if succed
 * @return LiteKitPSData output data, size of 192*192
 */
- (LiteKitPSData *) inferWithPixelBuffer:(CVPixelBufferRef)pixelBuffer
                               error:(NSError **)error;
```


### 3. 释放
iOS的人像分割接口ARC下不需要额外的释放操作


### 4. 预测结果数据模型
#### LiteKitPSData
人像分割能力的output数据结构定义LiteKitPSData，

```objective-c
/**
 * @desc PortraitSegmentor data for output
 */
 typedef struct LiteKitPortraitSegmentorData {
     struct LiteKitPortraitSegmentorDataShape {
         int n; // batch, =1
         int c; // channel, =1
         int h; // output height, =192
         int w; // output width, =192
     } dataShape; // output data shape
     uint8_t *data; // output data
 } LiteKitPSData;
```

LiteKitPSData中的dataShape：output数据的shape，其中：
n：批量数，目前批量处理图片数只能为1。
c：返回数据的channel数，此处目前返回的值为1，标识只返回了1个通道的数据。
h：返回数据的height，目前返回数据的height只为192。
w：返回数据的width，目前返回数据的width只为192。
```objective-c
    struct LiteKitPortraitSegmentorDataShape {
        int n; // batch, =1
        int c; // channel, =1
        int h; // output height, =192
        int w; // output width, =192
    } dataShape; // output data shape
    
```

LiteKitPSData中的data：output的数据部分，返回的数据大小w\*h=192\*192，只有1个channel，每一位数据为0或255，0表示该点不存在人像，255表示该点存在人像。
在实际的开发过程中，可以作为mask使用，具体如何将返回的数据应用于不同大小的图像或其他业务场景，这个问题需要开发者考虑。
```objective-c 
    uint8_t *data; // output data
```
### 5. Demo code

```objective-c
// 引入头文件
#import <LiteKitPortraitSegmentation/LiteKitPortraitSegmentor.h>

// 创建Predictor
LiteKitPortraitSegmentor *portraitSegmentor = [LiteKitPortraitSegmentor createPortraitSegmentorWithError:&error];

// 执行Predict、获取Output
LiteKitPSData *output = (LiteKitPSData *)[portraitSegmentor inferWithPixelBuffer:CMSampleBufferGetImageBuffer(sampleBuffer) error:nil];

// 释放Predictor
// Predictor不需要特殊的释放操作

```





<div id="AndroidAPI"></div>

## Android API

### 1. 创建实例
初始化人像分割的PortraitSegmentation

#### 参数
- config：目前作为预留接口，不需要进行额外的设置。
#### 返回值 
- 人像分割的PortraitSegmentation
```java
/**
 * @desc 对PortraitSegmentation进行初始化
 * @param config 初始化配置config
 */
public PortraitSegmentation(PortraitSegmentationConfig config)
```

### 2. 执行预测
人像分割PortraitSegmentation的预测接口
1）通过图像数据byte[]进行预测
#### 参数
- img_data：待预测图像数据，uint8_t 3 channel的RGB图像数据
- width：input图像的width
- height：input图像的height
#### 返回值
- 返回值为w\*h=192\*192的int[]，具体说明见[4. 返回值说明](#return)
```java
/**
 * @desc 通过byte数据进行预测
 * @param img_data 待预测的数据
 * @param imgWidth 图像Width
 * @param imgHeight 图像Height
 * @return int[] 预测结果
 */
public int[] predictor(byte[] img_data, int width, int height)
```

2）通过Bitmap进行预测
通过Bitmap进行预测的接口，其中接口通过Bitmap接受图像数据，返回值意义和1）通过图像数据byte[]预测接口相同。
```java
/**
 * @desc 通过bitmap数据进行预测
 * @param img_data 待预测的bitmap数据
 * @return int[] 预测结果
 */
public int[] predictor(Bitmap img_data)
```

### 3. 释放
释放人像分割资源
```java
/**
 * @desc 对人像分割LiteKitPortraitSegmentation进行释放
 */
public void release()
```

### 4. 返回值说明
#### return
人像分割返回为w\*h=192\*192的int[]，其中每一位的值为`value = ( a << 24 | 255 << 16 | 255 << 8 | 255 )`，
可以通过`Color.alpha(value)`获取到一个0～255的alpha值，0表示该点不存在人像，255表示该点存在人像


### 5. Demo Code
```java
// 引入头文件
import com.baidu.litekit.PortraitSegmentation;
import com.baidu.litekit.PortraitSegmentationConfig;

// 创建Config（模型地址）、创建Predictor
PortraitSegmentationConfig config = new PortraitSegmentationConfig(this);
PortraitSegmentation portraitSegmentation = new PortraitSegmentation(config);

// 执行预测，alpha_data为outputData
int[] alpha_data = portraitSegmentation.predictor(inputData, rgbaImage.getWidth(), rgbaImage.getHeight());

// 释放Predictor
portraitSegmentation.release();
```
