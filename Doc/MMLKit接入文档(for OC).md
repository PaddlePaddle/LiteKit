# MMLKit接入文档(for OC)
## 导入SDK
|能力 |对应SDK | 
|---|---|
| 人像分割| MMLPortraitSegmentation.framework | 
| 手势识别| MMLHandGestureDetection.framework | 
| 视频超分| MMLAIVideoSuperResolution.framework | 


## 添加第三方依赖
|依赖 | 版本| 
|---|---|
|opencv2| 3.4.1| 
|ProtocolBuffers| 1.0.0|
|ZipArchive| 1.0.0| 
|libpng| opencv3.4.1对应libpng版本| 
|libwebp| opencv3.4.1对应libwebp版本|
|libjpeg| opencv3.4.1对应libjpeg版本|

## 添加系统framework
无

## 配置环境
无

## 资源文件
将模型文件放置到工程适当位置，创建能力实例的时候需要读取模型文件。
![模型文件](/Doc/Resources/21_1.png)

## 使用方法
### 人像分割
引入头文件
```
#import <MMLPortraitSegmentation/MMLPortraitSegmentor.h>
```
创建Config（模型地址）、创建Predictor
```
NSString *modelPath = [[NSBundle mainBundle] pathForResource:@"Segmentation.bundle/humanseg-cpu" ofType:@"nb"];
self.portraitSegmentor = [MMLPortraitSegmentor create:modelPath error:&error];
```
执行Predict、获取Output
```
MMLPSData *output = (MMLPSData *)[self.portraitSegmentor inferWithPixelBuffer:CMSampleBufferGetImageBuffer(sampleBuffer) error:nil];
```
释放Predictor
```
Predictor不需要特殊的释放操作
```
### 手势识别
引入头文件
```
#import <MMLHandGestureDetection/MMLHandGestureDetector.h>
```
创建Config（模型地址）、创建Predictor
```
NSString *modelPath = [[[NSBundle mainBundle] bundlePath] stringByAppendingString:@"/HandGesture.bundle/"];
self.gestureRecognizer = [MMLHandGestureDetector createGestureDetectorWithModelPath:modelPath error:&error];
```
执行Predict、获取Output
```
//执行predict
[self.gestureRecognizer detectWithUIImage:self.image complete:^(MMLHandGestureDetectResult *result, NSError *error){
    //result为获取的output
}];
```
释放Predictor
```
Predictor不需要特殊的释放操作
```
### 视频超分
引入头文件
```
#import <MMLAIVideoSuperResolution/MMLAIVideoSuperResolution.h>
```
创建Config（模型地址）、创建Predictor
```
NSString *modelPath = [[[NSBundle mainBundle] bundlePath] stringByAppendingString:@"/SuperResolution.bundle"];
MMLVideoSuperResolutionConfig *config = [[MMLVideoSuperResolutionConfig alloc] init];
config.modelDir = modelPath;
MMLVideoSuperResolutionor *sVideo = [MMLVideoSuperResolutionor createInstanceWithConfig:config error:&error];
```
执行Predict、获取Output
```
// 执行predict，获取的newImg为output
UIImage *newImg = [self.superVideo superResolutionWithUIImage:self.image scale:1.0 error:&error];
```
释放Predictor
```
Predictor不需要特殊的释放操作
```
