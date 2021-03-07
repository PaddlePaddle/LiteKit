# LiteKit接入文档

[![License](https://img.shields.io/badge/license-MIT-green)](LICENSE)

LiteKit是基于端推理框架LiteKitCore和端推理引擎PaddleLite，面向移动端工程师的AI能力解决方案。
LiteKit旨在为客户端应用提供开箱即用的离线的AI能力，使产品快速的简单的接入AI能力，并将提供的AI能力应用于各种业务场景。

目前已经支持的人像分割、手势识别、视频超分均来自百度各个产品线，已上线功能中，中台化输出的AI能力。

## 快速体验

### 效果展示
| 手势识别 | 人像分割 |
| --- | --- |
| <div align=center><img width="320" height="480"  src="/Doc/Resources/1_5.gif"/></div> | <div align=center><img width="320" height="315"  src="/Doc/Resources/1_6.gif"/></div> |

视频超分
| 增强前 | 增强后 |
| --- | --- |
| ![SR](/Doc/Resources/1_7.png) | ![SR](/Doc/Resources/1_7_2.png) |
### 1. Android - 扫码安装<br>
![QR_Code](/Doc/Resources/1_3.png)

### 2. Demo Project 源码安装

```
git clone https://github.com/PaddlePaddle/LiteKit.git
```


#### iOS平台
示例工程中展示了人像分割、手势识别、视频超分能力。包括能力的接入demo和能力效果的展示。

###### 示例工程部署
```
cd LiteKit/LiteKitDemo/iOS/LiteKitDemo
pod install --repo-update
open LiteKitDemo.xcworkspace
```
运行工程可在真机测试机上查看效果。


#### Android平台
1. clone依赖SDK仓库作为LocalMaven仓库
```
git clone https://gitee.com/paddlepaddle/LiteKit
```

2. 修改LiteKitDemo工程local.properties（文件位置在`./LiteKitDemo/Android/LiteKitDemo/local.properties`)，添加MAVEN_REPO_LOCAL属性，值为第1步中clone的gitee仓库地址。
例如：
```
MAVEN_REPO_LOCAL = XXXXXXX
```

3. Gradle sync & Run



## 安装SDK

LiteKit依赖关系如下：
<p align="center"><img width="450" src="/Doc/Resources/1_4.png"/></p>

其中：
1. PaddleLite层，[PaddleLite](https://github.com/PaddlePaddle/Paddle-Lite)是一个高性能、轻量级、灵活性强且易于扩展的深度学习推理框架，LiteKitSDK的AI能力底层基于PaddleLite引擎实现。
2. LiteKitCore层，LiteKitCore是一种跨平台的，面向移动开发者的，AI工程化的综合解决方案。LiteKitCore作为端模型预测的统一接入层，目的是端模型的快速工程化集成，降低客户端RD在端运行AI模型的门槛和提升集成效率，同时也能更好实现基于端模型业务能力的快速横向输出。目前为提供了Objective C，Java，C++三种语言的API。
3. LiteKit层，视频超分，人像分割，手势识别，均称为LiteKit的业务SDK。每种业务SDK中，封装了对应AI能力的模型、预测的前后处理等逻辑。后续会产生更多覆盖其他AI场景的业务SDK。

### 1. 依赖
LiteKit由[MIT License](LICENSE)提供

| 功能 |依赖 | iOS版本| Android版本 |
|---|---|---|---|
| 手势识别 |LiteKitHandGestureDetection    | 0.2.0 | 0.2.0 | 
| 人像分割 |LiteKitPortraitSegmentation      | 0.2.0 | 0.2.0 |
| 视频超分 |LiteKitAIVideoSuperResolution  | 0.2.0 | 0.2.0 |

### 2. 安装
#### iOS 安装SDK
- 系统最低版本iOS 9.0
- 通过pod引入，[参考](/LiteKitDemo/iOS/LiteKitDemo/Podfile)
```ruby
# coding: utf-8
source 'https://github.com/CocoaPods/Specs.git'

platform :ios

target 'LiteKitDemo' do
  project './LiteKitDemo.xcodeproj'
    platform :ios, '10.0'

    pod 'LiteKitHandGestureDetection', '~> 0.2.0'
    pod 'LiteKitPortraitSegmentation', '~> 0.2.0'
    pod 'LiteKitVideoSuperResolution', '~> 0.2.0'
end

```

#### Android 安装SDK
- 系统最低API Level16（4.1版本）
1. 将依赖SDK仓库作为LocalMaven仓库
```
git clone https://gitee.com/paddlepaddle/LiteKit
```

2. 修改LiteKitDemo工程local.properties（例如`./LiteKitDemo/Android/LiteKitDemo/local.properties`)，添加MAVEN_REPO_LOCAL属性，值为第1步中clone的gitee仓库地址。
例如：
```
MAVEN_REPO_LOCAL = XXXXXXX
```

3. 在工程的gradle中增加配置LocalMaven，例如`./LiteKitDemo/Android/LiteKitDemo/build.gradle`
```groovy

allprojects {
    repositories {
        google()
        jcenter()
        maven{ url getLocalProperty("MAVEN_REPO_LOCAL") + "/Android/repository" }
    }
}

/**
 * 从local.properties文件中读取配置
 *
 * @param propertyName 读取的参数名
 * @return 配置value
 */
def getLocalProperty(String propertyName) {
    Properties properties = new Properties()
    try {
        properties.load(project.rootProject.file('local.properties').newDataInputStream())
    } catch (Exception e) {
        println "read local.properties failed${e.message}"
    }
    String value =  properties.get(propertyName);
    System.out.println("key: " + propertyName + ", value: " +  value)
    return value
}
```

4. 在app的gradle中增加配置引入SDK，例如`./LiteKitDemo/Android/LiteKitDemo/app/build.gradle`
```groovy

dependencies {
    implementation 'com.baidu.litekit:handgesturedetector:0.2.0'
    implementation 'com.baidu.litekit:portraitsegmentation:0.2.0'
    implementation 'com.baidu.litekit:videosuperresolution:0.2.0'
}
```


## API
LiteKit的AI能力，主要包含3类接口：创建、执行、释放。
使用时，先通过创建接口创建对应的预测引擎实例，之后可以通过执行接口进行预测，当使用完毕后需要对预测引擎实例进行释放。
<br>
其中执行接口通常有多个，可以接受不同格式的数据输入，以适应相机帧、图像、视频解码数据等不同也业务场景。

<p align="center"><img width="800" src="/Doc/Resources/1_8.png"/></p>

[人像分割 API文档](/Doc/LiteKit接口文档_人像分割.md) <br>
[手势识别 API文档](/Doc/LiteKit接口文档_手势识别.md) <br>
[视频超分 API文档](/Doc/LiteKit接口文档_视频超分.md) <br>


## 隐私说明
LiteKit目前版本不会收集任何用户数据和用户信息，也不需要申请用户的隐私权限。

## 交流与反馈
<p align="center">
    <img width="200" height="200"  src="https://user-images.githubusercontent.com/45189361/64117959-1969de80-cdc9-11e9-84f7-e1c2849a004c.jpeg"/>
    &#8194;&#8194;&#8194;&#8194;&#8194;
    <img width="200" height="200"  src="/Doc/Resources/1_9.png"/>
</p>
<p align="center">
&#8194;&#8194;&#8194;微信公众号&#8194;&#8194;&#8194;&#8194;&#8194;&#8194;&#8194;&#8194;
&#8194;&#8194;&#8194;&#8194;&#8194;&#8194;&#8194;&#8194;&#8194;官方技术交流微信群
</p>


## 版权和许可证
LiteKit由[MIT License](LICENSE)提供


## FAQ
[FAQ](/Doc/LiteKitFAQ.md)



# LiteKitCore接入文档
## 一、介绍
### 1. 背景
LiteKitCore是一种跨平台的，面向移动开发者的，AI工程化的综合解决方案。LiteKitCore作为端模型预测的统一接入层，目的是端模型的快速工程化集成，降低客户端RD在端运行AI模型的门槛和提升集成效率，同时也能更好实现基于端模型业务能力的快速横向输出。基于LiteKitCore的端AI预测能力，可以快速的基于不同宿主进行集成与部署。LiteKitCore主要功能如下：模型加载，预测能力，前后处理能力，业务数据到Backend Input/Output的转换，Backend无感知升级，性能统计，容错处理，任务队列管理，运行时资源调度，生命周期管理等。


### 2. 兼容性
支持平台：iOS、Android、Native C++ 。<br>


### 3. Demo Project
iOS示例工程包含了Native C++ API和Objective-C API的demo调用示例，Native C++ API和Objective-C API分别包含CPU和GPU两种backend，一共4种加载和预测的方式。针对C++API（在iOS上）和Objective-C API的CPU/GPU backend分别是对齐的，模型的加载和预测能力一致。但是CPU和GPU backend之间是隔离的，预测所使用的模型是不一样的。

#### 示例iOS工程部署
1. clone git仓库
```
git clone https://github.com/PaddlePaddle/LiteKit.git
```

2. 添加LiteKitCore依赖
按照[接入文档 for Objective-C API ](/Doc/LiteKitCore接入文档(for%20Objective-C%20API).md)构建产物`LiteKit.framework`放在`./LiteKitCore/LiteKitCoreDemo/iOS/Dependence/*`</br>
按照[接入文档 for Native C++ API on iOS](/Doc/LiteKitCore接入文档(for%20Native%20C%2B%2B%20API%20on%20iOS).md)构建产物`./LiteKitCore/LiteKitCore/C++/build-ios/product/release/*`，
*.h头文件到文件夹`./LiteKitCore/LiteKitCoreDemo/iOS/Dependence/LiteKitCoreNative/include/*.h`，liblitekit_framework_ios.a文件放到`./LiteKitCore/LiteKitCoreDemo/iOS/Dependence/LiteKitCoreNative/liblitekit_framework_ios.a`

3. 打开工程
```
cd ./LiteKitCore/LiteKitCoreDemo/iOS
pod install
open LiteKitCoreDemo.xcworkspace
```

|类名 | 说明 | 
|---|---|
|ViewController |LiteKit（Objective-C & C++）以GPU、CPU作为backend的load及predict的demo code |
| ViewController+LiteKitCore_CPP | LiteKit Native C++ API demo code|
| ViewController+LiteKitCore_OC | LiteKit Objective-C API demo code|

#### 部署Android示例工程  
1. 环境部署

| 环境 | 版本 | 
| ----- | ----- |
| NDK |[android-ndk-r16b](https://developer.android.google.cn/ndk/downloads/older_releases) |
| cmake | cmake 3.15.0 |

在~/.bashrc里指定cmake路径`ANDROID_CMAKE`和NDK路径`ANDROID_NDK`变量

2. 根据[接入文档 for Java API ](/Doc/LiteKitCore接入文档(for%20Java%20API).md), 生成`LiteKitCore-debug.aar`，放置在`./LiteKitCore/LiteKitCoreDemo/Android/app/libs/litekitcore-debug.aar`路径下。
3. 需要[下载](https://gitee.com/paddlepaddle/LiteKit/tree/main/Android/LiteKitCoreDemoDependency/opencv/0.1.0)Demo依赖的opencv.so和libgnustl_shared.so，放置在`./LiteKitCore/LiteKitCoreDemo/Android/app/src/main/cpp/libs`路径下。

## 二、接口文档
[接口文档 for Native C++ API](/Doc/LiteKitCore接口文档(for%20Native%20C%2B%2B%20API).md)
<br>
[接口文档 for Objective-C API ](/Doc/LiteKitCore接口文档(for%20Objective-C%20API).md)
<br>
[接口文档 for Java API ](/Doc/LiteKitCore接口文档(for%20Java%20API).md)

## 三、接入文档
[接入文档 for Objective-C API ](/Doc/LiteKitCore接入文档(for%20Objective-C%20API).md)
<br>
[接入文档 for Native C++ API on iOS](/Doc/LiteKitCore接入文档(for%20Native%20C%2B%2B%20API%20on%20iOS).md)
<br>
[接入文档 for Native C++ API on Android](/Doc/LiteKitCore接入文档(for%20Native%20C%2B%2B%20API%20on%20Android).md)
<br>
[接入文档 for Java API ](/Doc/LiteKitCore接入文档(for%20Java%20API).md)



## FAQ
[FAQ](/Doc/LiteKitCoreFAQ.md)

