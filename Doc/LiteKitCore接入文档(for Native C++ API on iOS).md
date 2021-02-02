# LiteKitCore接入文档 (for Native C++ API on iOS)


## 一、Linux / macOS编译
### 1. 构建环境
Native C++ iOS产物依赖及版本：
|环境依赖 | 版本 |
|---|---|
|cmake| 3.15.0 |  
|Xcode| 11.3以上 |  

[官网下载](https://github.com/Kitware/CMake/releases/tag/v3.15.0)安装 [cmake 3.15.0](https://github.com/Kitware/CMake/releases/download/v3.15.0/cmake-3.15.0-Darwin-x86_64.dmg)<br>  
[官网下载](https://developer.apple.com/download/more/)安装[Xcode 11.3](https://download.developer.apple.com/Developer_Tools/Xcode_11.3/Xcode_11.3.xip)（或从AppStore下载11.3以上版本Xcode）


### 2. 构建选项
Cmake构建参数选择：<br>
DCMAKE_OSX_ARCHITECTURES =  armv7、arm64<br>
DCMAKE_BUILD_TYPE = release



### 3. 构建步骤
```
git clone https://github.com/PaddlePaddle/LiteKit.git
cd ./LiteKitCore/LiteKitCore/C++/build-ios 
sh product_build.sh 
 
构建产物位置：
产物目录PRODUCT_DIR = ./LiteKitCore/LiteKitCore/C++/build-ios/product
产物头文件目录PRODUCT_HEADER_DIR = PRODUCT_DIR/release/include/
产物静态库目录PRODUCT_FINAL_DIR = PRODUCT_DIR/release/libpaddle_api_light_bundled.a
```

## 二、集成
### 1. 导入SDK
将LiteKitNative的可执行文件和头文件放在工程目录下
![图片](/Doc/Resources/3_1.png)

### 2. 添加第三方依赖
无

### 3. 添加系统framework
将使用Native C++的类改名为.mm结尾，以便使用Object-C++ source模式编译

### 4. 配置环境
1) Library Search Path和Header Search Paths中分别添加对Native C++ API的引用
![图片](/Doc/Resources/3_2.png)

2) Other Linker Flags添加 -l"litekit_framework_ios"

### 5. 资源文件
如果使用PaddleMobileGPU作为backend，需要将paddle-mobile-metallib.metallib文件拷贝至main bundle下
![图片](/Doc/Resources/3_3.png)

## 三、使用
   LiteKit Native C++ API执行推理的时候。
   <br>首先，需要确定模型、所使用的backend、模型和backend是否匹配、以及其他config参数，并创建config。
   <br>然后，通过config 对Service进行load，Service直接管理预测backend，用户仅需要持有/释放service，不需要对backend的实例进行直接管理。Service直接对用户提供填充input，预测、获取output服务。
   <br>Service提供指针类型创建（new）和std::shared_ptr创建两种方式，其中指针类型创建（new）需要手动delete释放Service内存。


### 1) 引入头文件
```cpp
// import LiteKit Native C++ header
#import "litekit_inference_api.h"
```
### 2) 创建Config （含模型转换）
```cpp
// 创建 LiteKit Native C++ Config

// CPU config
litekit_framework::LiteKitConfig config;
config.machine_type = litekit_framework::LiteKitConfig::MachineType::PaddleLite;

litekit_framework::LiteKitConfig::PaddleLiteConfig paddle_config;
paddle_config.powermode = litekit_framework::LiteKitConfig::PaddleLiteConfig::PaddleLitePowerMode::LITE_POWER_NO_BIND;
paddle_config.threads = 1;
paddle_config.model_type = litekit_framework::LiteKitConfig::PaddleLiteConfig::LITE_MODEL_FROM_FILE;

paddle_config.model.model_from_file.data = ""; // 模型文件地址String char *
paddle_config.model.model_from_file.size = length; // 模型文件地址String length

// GPU config
litekit_framework::LiteKitConfig config;
config.machine_type = litekit_framework::LiteKitConfig::MachineType::PaddleiOSGPU;

litekit_framework::LiteKitConfig::PaddleiOSGPUConfig paddle_gpu_config;
config.machine_config.paddle_ios_gpu_config = paddle_gpu_config;

config.model_file_name = ""; // 模型文件地址String char *
config.param_file_name = length; // 模型文件地址String length
config.modelUrl = [gpuDir UTF8String]; // mv6s是多输出模型
```
### 3) 创建Service
```cpp
//创建service

//std::shared_ptr类型service
std::shared_ptr<litekit_framework::LiteKitMachineService> service = litekit_framework::CreateLiteKitMachineService(config);

//指针类型service
litekit_framework::LiteKitMachineService *service = new litekit_framework::LiteKitMachineService();
int result = service->load(config);
```
### 4) 前处理
```cpp
// 进行需要的前处理
float *inputDemoData;// example input数据
```

### 5) 创建Input
```cpp
// create input data
//  data shape
litekit_framework::shape_t shape({1, 1, 192, 192});// example shape
// 获取第0个input
std::unique_ptr<litekit_framework::LiteKitData> inputData = service->getInputData(0);
litekit_framework::LiteKitTensor *aInput = inputData->LiteKitTensor;
//resize input
aInput->Resize(shape);
// 获取input存储数据的指针
auto *data = aInput->mutable_data<float>();// example：float数据
//填充input数据
memcpy(data, inputDemoData, inputDemoDataSize*sizeof(float));// example float数据
```
### 6) Machine执行predict
```cpp
// 执行预测
service->run();
```

### 7) 读取output
```cpp
// get output
// 读取第0个output
std::unique_ptr<const litekit_framework::LiteKitData> outputData = service->getOutputData(0);
const litekit_framework::LiteKitTensor *output_tensor = outputData->LiteKitTensor;
// 读取output的shape
int64_t outputDataSize = demo_shapeProduction(output_tensor->shape());
//计算数据长度
int64_t dataSizex = outputData->dataLength;
float *output = (float *)malloc(sizeof(float)*outputDataSize); // example output data
// 读取output数据
memcpy(output, output_tensor->data<float>(), sizeof(float)*outputDataSize);
```
 
### 8) 后处理
```cpp
//读取output数据后，进行需要的后处理
float *output = (float *)malloc(sizeof(float)*outputDataSize); // example output data

```

### 9) 释放Machine
```cpp
// 如果是指针类型service，释放service
delete service;
```

## 常见错误说明
```cpp
加载常见错误说明
/**
 * 模型加载错误，引擎创建错误
 */
enum ErrorCode {
    SUCCESS = 0,

    ERR_PARAM = -1, // iOS GPUbackend预测的时候，如果inputData的rawData==nulltr || datalength<=0的时候，返回该错误。

    LOAD_ERR_OTHER = -11,// paddle Lite backend的时候，如果LiteKit的header和库不匹配，或者因为模型和Lite不匹配，以及其他一些未知原因加载失败的时候，返回该错误。

    RUN_ERR_PREDICT = -23, // iOS GPU backend 预测的时候，执行预测失败的时候，返回该错误。

    RUN_ERR_MACHINE_TYPE = -24, // LiteKit Service load的时候传入错误的machine_type，或者iOS平台上使用了不支持GPU的差异化版本加载GPU service的时候，返回该错误。

    RUN_ERR_MACHINE_HANDLE = -25, // LiteKit service 执行predict的时候，并没有找到backend（或load的时候创建backend失败）的时候，返回该错误

};
```
