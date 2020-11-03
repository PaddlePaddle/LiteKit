# MMLKit接入文档
## 1.介绍
### 背景
基于Paddle⽣态的AI开放能⼒，⾯向客户端⼯程师的产品应⽤解决⽅案。 提供开箱即用的AI能力，支持产品快速接入。目前已输出人像分割、手势识别、视频超分能力。

### 兼容性
支持平台：iOS、Android。
<br>
支持语言：Objective-C、Java。

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
快速体验Android - 扫码安装
[QR_Code](/Doc/Resources/1-3.png)
<br>

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
[接口文档 for iOS](/Doc/MMLKit接口文档(for%20iOS).md) <br>
[接口文档 for Android](/Doc/MMLKit接口文档(for%20Android).md)


## 3. 接入文档
[接入文档 for iOS](/Doc/MMLKit接入文档(for%20iOS).md) <br>
[接入文档 for Android](/Doc/MMLKit接入文档(for%20Android).md)









# MMLCore接入文档
## 一、介绍
### 1. 背景
MMLCore是一种业界领先的，跨平台的，面向移动开发者的，AI工程化的综合解决方案。MMLCore作为端模型预测的统一接入层，目的是端模型的快速工程化集成，降低客户端RD在端运行AI模型的门槛和提升集成效率，同时也能更好实现基于端模型业务能力的快速横向输出。基于MMLCore的端AI预测能力，可以快速的基于不同宿主进行集成与部署。MMLCore主要功能如下：模型加载，预测能力，前后处理能力，业务数据到Backend Input/Output的转换，Backend无感知升级，性能统计，容错处理，任务队列管理，运行时资源调度，生命周期管理等。


### 2. 兼容性
支持平台：iOS、Android、Native C++ 。<br>
支持Backend：支持PaddleMobile/Paddle-Lite/BML(LR/GBDT)等Backend。 


### 3. 依赖和体积
#### iOS平台
###### Native C++ API

| 依赖 | 版本 | 体积 | 
|---|---|---|
| MML Native | 0.0.9 | 51.5MB(armv7), 51MB(arm64) |

###### Objective-C API
|依赖| 版本|体积 | 
|---|---|---|
|MML|0.0.9|3.3MB|
|opencv|3.4.1|128.6MB|
|paddleLite|1.0.0|102.2MB|
|paddle_mobile|1.0.0|14.1MB|
|ProtocolBuffers|1.0.0|18.7MB|
|ZipArchive|1.0.0|3.9MB|


#### Java API 依赖
|依赖|版本| 体积 | 
|---|---|---|
| MML Native|-|51.5MB(armv7), 51MB(arm64) |

### 4. Demo Project
iOS示例工程包含了Native C++ API和Objective-C API的demo调用示例，Native C++ API和Objective-C API分别包含CPU和GPU两种backend，一共4种加载和预测的方式。针对C++API（在iOS上）和Objective-C API的CPU/GPU backend分别是对齐的，模型的加载和预测能力一致。但是CPU和GPU backend之间是隔离的，预测所使用的模型是不一样的。

#### 示例iOS工程部署
1.组装工程 
```
git clone https://github.com/PaddlePaddle/LiteKit.git
cd PaddleMMLCore/MMLDemo/iOS
open MMLDemo.xcodeproj
```
2.配置调试真机及Apple ID
![图片](/Doc/Resources/1_1.png)

3.运行示例工程
![图片](/Doc/Resources/1_2.png)
|类名 | 说明 | 
|---|---|
|ViewController |MML（Objective-C & C++）以GPU、CPU作为backend的load及predict的demo code |
| ViewController+MMLCore_CPP | MML Native C++ API load demo code|
| ViewController+MMLCore_OC | MML Objective-C API load demo code|

#### 部署Android示例工程  
1. 首先编译[MMLCore/MML/C++](MMLCore/MML/C%2B%2B/README.md), 生成`libmml_framework.so`
2. 然后编译[MMLCore/MML/Android](MMLCore/MML/Android/README.md), 生成`mmlcore-debug.aar`
3. 最后编译[MMLCore/MMLDemo/Android](MMLCore/MMLDemo/Android/README.md), 通过Android studio 打开项目即可运行demo

## 二、接口文档
[接口文档 for Native C++ API](/Doc/MMLCore接口文档(for%20Native%20C%2B%2B%20API).md)
<br>
[接口文档 for Objective-C API ](/Doc/MMLCore接口文档(for%20Objective-C%20API).md)
<br>
[接口文档 for Java API ](/Doc/MMLCore接口文档(for%20Java%20API).md)

## 三、接入文档
[接入文档 for Objective-C API ](/Doc/MMLCore接入文档(for%20Objective-C%20API).md)
<br>
[接入文档 for Native C++ API on iOS](/Doc/MMLCore接入文档(for%20Native%20C%2B%2B%20API%20on%20iOS).md)
<br>
[接入文档 for Native C++ API on Android](/Doc/MMLCore接入文档(for%20Native%20C%2B%2B%20API%20on%20Android).md)
<br>
[接入文档 for Java API ](/Doc/MMLCore接入文档(for%20Java%20API).md)




