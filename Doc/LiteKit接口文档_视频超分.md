# 视频超分接口文档
视频超分能力是由LiteKit VideoSuperResolution SDK提供的，对图像进行超分辨率处理的能力。在视频中，对于通常的480p超分到720p场景可以达到25FPS。

### 目录
<a href="#iOSAPI">iOS API</a>
<br>
<a href="#AndroidAPI">Android API</a>
<br>

<div id="iOSAPI"></div>

## iOS API

### 1. 创建实例
视频超分能力inferencer，通过createInstanceWithConfig创建完成inferencer之后，可以通过
UIImage和LiteKitYUV420Data两种数据作为input执行超分。

#### 参数
- error：创建过程中如果发生错误，返回错误信息。如果成功创建，不会赋值。
#### 返回值
- 创建成功的手势识别inferencer

```objective-c
LiteKitVideoSuperResolutionor.h

/**
 * @brief initialize and return a instance of video super resolutionor
 *
 * @param error if an error occurs, this param will carry the information
 * @return a instance of gesture detector
 */
+ (instancetype)createVideoSuperResolutionorWithError:(NSError **)error;
```

### 2. 执行预测
1）通过LiteKitYUV420Data数据进行预测，关于LiteKitYUV420Data的详细说明见[4. 预测结果数据模型](#LiteKitYUV420Data)

#### 参数
- inputData：input的待超分数据
- outputData：超分返回的output数据，其中对应YUV数据内存需要在调用前进行分配。超分结果的size取决于outputData设置的size。
- error：如果超分过程中产生错误，则会对error进行赋值，此时outputData中的数据无效。如果error没有值，则表示超分成功。
#### 返回值
- BOOL：true表示超分成功，false表示超分失败。只有超分成功的时候outputData中的YUV数据才是有效数据。

```objective-c
/**
 * @brief execute videoSuperSolution
 *
 * @param inputData input data, struct LiteKitYUV420Data
 * @param outputData output data
 * @param error errorcode and error reason
 * @return BOOL YES for succeed，NO for failure
 */
- (BOOL)superResolutionWithInputData:(const LiteKitYUV420Data *)inputData
                          outputData:(LiteKitYUV420Data *)outputData
                               error:(NSError **)error;
```

2）通过UIImage进行预测

#### 参数
- inputImage：输入的图像
- scale：超分过程中缩放的倍数，一般scale>=1。scale=1的时候，输入输出大小相等，进通过超分能力对图像进行重建。scale>1的时候，会对input进行scale后进行重建。
- error：如果超分过程中产生错误，则会对error进行赋值，此时outputData中的数据无效。如果error没有值，则表示超分成功。

#### 返回值
- UIImage：输出的图像

```objective-c
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
```

### 3. 释放
iOS的视频超分接口ARC下不需要额外的释放操作



### 4. 预测结果数据模型
#### LiteKitYUV420Data
LiteKitYUV420Data所包含的内容应当为一个图像的YUV420数据。
对于视频超分能力来说，一个LiteKitYUV420Data应该包含视频解码后的一帧的YUV420信息。
```objective-c
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
} LiteKitYUV420Data;//video 420 frame data
```

LiteKitYUV420Data 中的数据部分，储存在y_data、u_data、v_data中，数据类型为uint8_t。
```objective-c
//YUVdata
uint8_t* y_data;
uint8_t* u_data;
uint8_t* v_data;
```

width和height表示图像的长宽，y_stride、u_stride、v_stride表示图像的步长（行宽）。
```objective-c
// frame width
int width;
// frame height
int height;
// data length (include Padding)
int y_stride;
int u_stride;
int v_stride;
```

注意以下两点：
- 图像数据为YUV420格式
- 视频解码后的图像帧数据，行中右侧通常会包含padding部分
所以，通常：<br>
    - y_data的length = y_stride\*height，<br>
    - u_data的length = u_stride\*height，<br>
    - v_data的length = v_stride\*height，<br>
    - 2\*u_stride = 2\*v_stride = y_stride >= width。

#### 5. Demo code
```objective-c
// 引入头文件
#import <LiteKitAIVideoSuperResolution/LiteKitAIVideoSuperResolution.h>

// 创建Predictor
LiteKitVideoSuperResolutionor *sVideo = [LiteKitVideoSuperResolutionor createVideoSuperResolutionorWithError:&error];

// 执行predict，获取的newImg为output
UIImage *newImg = [sVideo superResolutionWithUIImage:self.image scale:1.0 error:&error];

// 释放Predictor
// Predictor不需要特殊的释放操作
```

<div id="AndroidAPI"></div>

## Android API

### 1. 创建实例
创建视频超分能力的inferencer

#### 参数
- Context  上下文信息
#### 返回值
- long：视频超分inferencer的handler，在预测和释放的接口中需要使用。
```java
public static long init(Context context);
```


### 2. 执行预测
1）通过Bitmap进行预测
#### 参数
- handle：视频超分inferencer的handler
- in：输入的图像
- scale：超分中对图像scale的倍数

#### 返回值
- Bitmap：超分后返回的Bitmap

```java

/**
 * @desc 对bitmap执行超分
 * @param handle VideoSuperResolution的handler
 * @param in input数据bitmap
 * @param scale scale倍数
 * @return Bitmap output数据bitmap
 */
public static native Bitmap nativePredictBitmap(long handle, Bitmap in, float scale);

```

2）通过数据byte[]进行预测
#### 参数
handle：视频超分inferencer的handler
rgbabytearray：图像rgba格式的unsigned char channel 4的数据
height：input图像的height
width：input图像的width
scale：超分中对图像scale的倍数

#### 返回值
- byte[]：超分后返回的byte[]，内容为图像的RGBA格式的数据，

```java

/**
 * @desc 对bitmap执行超分
 * @param handle VideoSuperResolution的handler
 * @param in rgbabytearray数据rgba的数据数组
 * @param height input的height
 * @param width input的width
 * @param scale scale倍数
 * @return byte[] output数据
 */
public static native byte[] nativePredictRGBA(long handle, byte[] rgbabytearray, int height, int width, float scale);
    
```

### 3. 释放
使用完成之后，对视频超分进行释放

#### 参数
- handle：视频超分inferencer的handler

```java

/**
 * @desc 对VideoSuperResolution进行释放
 */
public static native void nativeReleaseSrSdk(long handle);

```

### 4. DemoCode
```java

// 引入头文件
import com.baidu.litekit.VideoSuperResolution;

// 创建Predictor
Long handle = VideoSuperResolution.init(this);

// 执行Predict、获取Output

Bitmap lowBitmap; // input待超分bitmap
int byteCount = lowBitmap.getByteCount();
ByteBuffer buf  = ByteBuffer.allocate(byteCount);
lowBitmap.copyPixelsToBuffer(buf);
byte[] rgba = buf.array();
byte[] targetRGBA = VideoSuperResolution.nativePredictRGBA(
                handle,      // 创建的predictorhandler
                rgba,        // 待超分rgba数据
                lowBitmap.getHeight(),
                lowBitmap.getWidth(),
                scale         // scale 倍数
        );

// or

Bitmap bitmap2 = VideoSuperResolution.nativePredictBitmap(
                handle,       // 创建的predictorhandler
                lowBitmap,    // 待超分bitmap
                scale         // scale 倍数
        );

// release SR machine
VideoSuperResolution.nativeReleaseSrSdk(handle);
```
