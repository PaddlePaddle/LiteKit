# LiteKitCore接入文档 (for Native Objective-C API)


## 一、编译
### 1. 构建环境
Native C++ iOS产物依赖及版本：
|环境依赖 | 版本 |
|---|---|  
|Xcode| 11.3以上 |  

[官网下载](https://developer.apple.com/download/more/)安装[Xcode 11.3](https://download.developer.apple.com/Developer_Tools/Xcode_11.3/Xcode_11.3.xip)（或从AppStore下载11.3以上版本Xcode）


### 2. 构建选项
OTHER_CFLAGS="-fembed-bitcode"



### 3. 构建步骤
```
git clone https://github.com/PaddlePaddle/LiteKit.git
cd ./LiteKitCore/LiteKitCore/C++/build-ios
sh product_build.sh 
 
构建产物位置：
产物目录PRODUCT_DIR = ./output/LiteKit.framework
```

## 二、集成
### 1. 导入SDK
在build phases中导入LiteKit Objective-C SDK： LiteKit.framework
![图片](/Doc/Resources/2_1.png)

### 2. 添加第三方依赖
在build phases中添加LiteKit Objective-C SDK相关依赖
|依赖| 版本|
|---|---|---|---|
|opencv|3.4.1|
|paddleLite|1.0.0|
|paddle_mobile|1.0.0|
|ProtocolBuffers|1.0.0|
|ZipArchive|1.0.0|

### 3. 添加系统framework
在工程中添加一个swift文件，以支持swift相关库的使用。如果已经在使用swift，跳过此步。
![图片](/Doc/Resources/2_2.png)

### 4. 配置环境
检查build setting中的framework Search Paths中是否已经正确配置引用的frameowkr paths。
![图片](/Doc/Resources/2_3.png)

### 5. 资源文件
如果使用PaddleMobileGPU作为backend，需要将paddle-mobile-metallib.metallib文件拷贝至main bundle下
![图片](/Doc/Resources/2_4.png)

## 三、使用
LiteKit Objective-C API执行推理的时候。
   <br>首先，需要确定模型、所使用的backend、模型和backend是否匹配、以及其他config参数，并创建config。
   <br>然后，通过Service的loadMachineWithConfig:error:对config进行加载，生成预测的machine，用户仅需要持有/操作machine，不需要对backend的实例进行直接管理。machine直接对用户预测的服务。


### 1) 引入头文件
```
// import LiteKit Objective-C header

#import <LiteKit/LiteKit.h> // Umbrella header

#import <LiteKit/LiteKitMachineConfig.h> // 默认的、基础的LiteKitMachine Config

#import <LiteKit/LiteKitPaddleConfig.h> // GPU backend需要使用的config

#import <LiteKit/LiteKitMachineService.h> // LiteKitMachineService，用于加载config，创建预测machine

#import <LiteKit/LiteKitDataProcess.h> // LiteKit提供的前后处理相关工具，包括图片旋转、缩放、剪裁等
```
### 2) 创建Config （含模型转换）
```
// 创建 LiteKit Native C++ Config

// CPU config
// construct LiteKit config
LiteKitMachineConfig *machineConfig = [[LiteKitMachineConfig alloc] init];
machineConfig.modelPath = @""; // example：模型路径
machineConfig.machineType = LiteKitPaddleCPU;

/************************************************************/
// GPU config
PaddleGPUConfig *modelConfig = [[PaddleGPUConfig alloc] init];
modelConfig.dims = @[@(kLiteKitInputBatch), @(kLiteKitInputChannel), @(kLiteKitInputHeight), @(kLiteKitInputWidth)];
modelConfig.useMPS = YES;
modelConfig.useAggressiveOptimization = YES;
modelConfig.computePrecision = PrecisionTypeFloat16;
modelConfig.modelPrecision = PrecisionTypeFloat32;
modelConfig.metalLibPath = [NSBundle.mainBundle pathForResource:@"paddle-mobile-metallib" ofType:@"metallib"];
modelConfig.metalLoadType = LoadMetalInCustomMetalLib;

// construct inf engine config
LiteKitPaddleConfig *paddleConfig = [[LiteKitPaddleConfig alloc] init];
paddleConfig.netType = CustomNetType;
paddleConfig.paddleGPUConfig = modelConfig;

// construct LiteKit config
LiteKitMachineConfig *machineConfig = [[LiteKitMachineConfig alloc] init];
machineConfig.modelPath = @""; // example：模型路径
machineConfig.engineConifg = paddleConfig;
machineConfig.machineType = LiteKitPaddleGPU;

```
### 3) 创建Service
```
//创建machine,加载模型
NSError *aError = nil;
LiteKitMachineService *service = [[LiteKitMachineService alloc] init];
LiteKitBaseMachine *litekitMachine = [service loadMachineWithConfig:machineConfig error:&aError];
//如果 aError==nil && litekitMachine!=nil,表示创建成功 
```
### 4) 前处理
```
// 进行需要的前处理
float *inputDemoData;// example input数据
```

### 5) 创建Input
```
// create input data
// 数据准备
NSArray *dims = @[@(n), @(c), @(h), @(w)]; // input的dims
// 创建 inputData
LiteKitShapedData *shapeData = [[LiteKitShapedData alloc] initWithData:inputDemoData dataSize:n*w*h*c dims:dims]; 
LiteKitData *inputData = [[LiteKitData alloc] initWithData:shapeData type:TLiteKitDataTypeLiteKitShapedData];
```
### 6) Machine执行predict
```
// 执行预测
// run sync
NSError *error = nil;
LiteKitData *outputData = [litekitMachine predictWithInputData:inputData error:&error];
```

### 7) 读取output
```
预测返回LiteKitData *outputData即为outputData
```
 
### 8) 后处理
```
根据output获取的LiteKitData类型及数据进行需要后处理
```

### 9) 释放Machine
```
ARC下不需要特别的释放操作
```

## 常见错误说明
```
加载常见错误说明
/// service load阶段错误值枚举
typedef NS_ENUM(NSInteger,LiteKitMachineServiceLoadErrorCode) {
    LiteKitMachineServiceLoadMachineTypeError = 0,  // 错误的machine类型，loadMachineWithConfig阶段，传入了不存在的machineType，或者使用的是差异化的剪裁版本，传入了剪裁掉的backend对应的machineType的时候，会返回该错误。
    LiteKitMachineServiceLoadNotSupportSimulator,   // 不支持模拟器，在模拟器上运行的时候，会返回该错误。
    LiteKitMachineServiceLoadNotSupportArchitecture,// 不支持的处理器架构，在Mac OS上运行的时候，会返回该错误。
    LiteKitMachineServiceLoadWrongConfig,           // 错误的配置，创建GPUbackend的时候，如果engineConifg为空，或类型错误的时候，会返回该错误。
    LiteKitMachineServiceLoadNoModelFile,           // 没有模型文件
    LiteKitMachineServiceLoadNoModelPointer,        // 内存指针为空，创建GPUbackend的时候，如果LiteKitMachineConfig的modelPath为空， 且engineConifg的paramPointer和modelPointer均为空的时候，会返回该错误。modelPath和engineConifg这两处至少有一处需要对模型进行配置。
};
```
