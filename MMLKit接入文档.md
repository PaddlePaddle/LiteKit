# MMLKit接入文档
## 1.介绍
### 背景
基于Paddle⽣态的AI开放能⼒，⾯向客户端⼯程师的产品应⽤解决⽅案。 提供开箱即用的AI能力，支持产品快速接入。目前已输出人像分割、手势识别、视频超分能力。

### 兼容性
支持平台：iOS、Android。
<br>
支持语言：OC、Java。

### 依赖和体积
#### iOS平台
|依赖 | 版本| 体积 |
|---|---|---|
|MMLHandGestureDetection| 0.0.9| 927KB |
|MMLPortraitSegmentation| 0.0.9| 10.9MB |
|MMLAIVideoSuperResolution| 0.0.9| 1.7MB |
|ProtocolBuffers| 0.0.9| 17.7MB |
|ZipArchive| 0.0.9| 3.9MB |
|opencv2| 3.4.1| 128.6MB |
|libpng| opencv3.4.1对应libpng版本| 3.1MB |
|libwebp| opencv3.4.1对应libwebp版本| 7.5MB | 
|libjpeg| opencv3.4.1对应libjpeg版本| 3.9MB | 


#### Android平台
|依赖 | 版本| 体积 |
|---|---|---|
|gesturerecognize | 0.0.9 | 50KB |
|superresolution | 0.0.9 | 45KB |
|portraitsegmentation | 0.0.9 | 437KB |
|mmlkitdependency | 0.0.9 | 3.4MB |
|thirdpartydependency | 0.0.9 | 11.8MB |

### Demo Project
#### iOS平台
###### 示例工程说明
示例工程中展示了人像分割、手势识别、视频超分能力。包括能力的接入demo和能力效果的展示。

###### 示例工程部署
```
git clone https://github.com/PaddlePaddle/LiteKit.git
cd MMLKit/MMLKitDemo/iOS/MMLKitDemo
pod install --repo-update
open MMLKitDemo.xcworkspace
```
运行工程可在真机测试机上查看效果。


#### Android平台
1. 打开Android Studio，点击File->Open...，选择MMLKitDemo/Android目录
2. 参考[MMLKit README](/MMLKit/MMLKitDemo/Android/README.md)文档，下载并放置依赖aar至对应位置
3. 点击Run安装运行到真机上（Demo中视频检测依赖摄像头输入）



## 2. 接口文档
[接口文档 for OC](/Doc/MMLKit接口文档(for%20OC).md)


## 3. 接入文档
[接入文档 for OC](/Doc/MMLKit接入文档(for%20OC).md)








