# MMLCore接入文档 (for Native C++ API on Android)


## 一、编译
### 1. 构建环境
Native C++ Android产物依赖及版本：
|环境依赖 | 版本 |
|---|---|  
| ndk | android-ndk-r16b |
| c++ | ANDROID_STL=c++_shared |



### 2. 构建选项
|cmake构建选项 | 选项 |
|---|---|  
| -DANDROID_TOOLCHAIN_NAME | llvm |
| -DANDROID_TOOLCHAIN | clang |
|-DANDROID_ARM_NEON | ON |
|-DANDROID_STL | c++_shared  |


### 3. 构建步骤
根据文档[MMLCore/MML/C++](MMLCore/MML/C%2B%2B/README.md), 生成`libmml_framework.so`

## 二、集成
### 1. 导入SDK
将MMLNative的可执行文件`libmml_framework.so`和头文件放在工程目录下


### 2. 添加第三方依赖
无

### 3. 添加系统framework
无

### 4. 配置环境
无

### 5. 资源文件
无

## 三、使用
   MML Native C++ API执行推理的时候。
   <br>
   首先，需要确定模型、所使用的backend、模型和backend是否匹配、以及其他config参数，并创建config。
   <br>
   然后，通过config 对Service进行load，Service直接管理预测backend，用户仅需要持有/释放service，不需要对backend的实例进行直接管理。
   <br>
   Service直接对用户提供填充input，预测、获取output服务。
   <br>
   Service提供指针类型创建（new）和std::shared_ptr创建两种方式，其中指针类型创建（new）需要手动delete释放Service内存。
    

### 1) 引入头文件
```
// import MML Native C++ header
#import "mml_inference_api.h"
```
### 2) 创建Config （含模型转换）
```
// 创建 MML Native C++ Config

    // CPU/GPU由模型决定
    mml_framework::MMLConfig config;
    config.machine_type = mml_framework::MMLConfig::MachineType::PaddleLite;

    mml_framework::MMLConfig::PaddleLiteConfig paddle_config;
    paddle_config.powermode = mml_framework::MMLConfig::PaddleLiteConfig::PaddleLitePowerMode::LITE_POWER_NO_BIND;
    paddle_config.threads = 1;
    paddle_config.model_type = mml_framework::MMLConfig::PaddleLiteConfig::LITE_MODEL_FROM_FILE;

    paddle_config.model.model_from_file.data = ""; // 模型文件地址String char *
    paddle_config.model.model_from_file.size = length; // 模型文件地址String length
    
```
### 3) 创建Service
```
//创建service

    //std::shared_ptr类型service
    std::shared_ptr<mml_framework::MMLMachineService> service = mml_framework::CreateMMLMachineService(config);

    //指针类型service
    mml_framework::MMLMachineService *service = new mml_framework::MMLMachineService();
    int result = service->load(config);
```
### 4) 前处理
```
// 进行需要的前处理
    float *inputDemoData;// example input数据
```

### 5) 创建Input
```
// create input data
    //  data shape
    mml_framework::shape_t shape({1, 1, 192, 192});// example shape
    // 获取第0个input
    std::unique_ptr<mml_framework::MMLData> inputData = service->getInputData(0);
    mml_framework::MMLTensor *aInput = inputData->mmlTensor;
    //resize input
    aInput->Resize(shape);
    // 获取input存储数据的指针
    auto *data = aInput->mutable_data<float>();// example：float数据
    //填充input数据
    memcpy(data, inputDemoData, inputDemoDataSize*sizeof(float));// example float数据
```
### 6) Machine执行predict
```
// 执行预测
    service->run();
```

### 7) 读取output
```
// get output
    // 读取第0个output
    std::unique_ptr<const mml_framework::MMLData> outputData = service->getOutputData(0);
    const mml_framework::MMLTensor *output_tensor = outputData->mmlTensor;
    // 读取output的shape
    int64_t outputDataSize = demo_shapeProduction(output_tensor->shape());
    //计算数据长度
    int64_t dataSizex = outputData->dataLength;
    float *output = (float *)malloc(sizeof(float)*outputDataSize); // example output data
    // 读取output数据
    memcpy(output, output_tensor->data<float>(), sizeof(float)*outputDataSize);
```
 
### 8) 后处理
```
//读取output数据后，进行需要的后处理
    float *output = (float *)malloc(sizeof(float)*outputDataSize); // example output data

```

### 9) 释放Machine
```
// 如果是指针类型service，释放service
    delete service;
```

## 常见错误说明
```
加载常见错误说明
    /**
     * 模型加载错误，引擎创建错误
     */
    enum ErrorCode {
        SUCCESS = 0,
        
        ERR_PARAM = -1, // iOS GPUbackend预测的时候，如果inputData的rawData==nulltr || datalength<=0的时候，返回该错误。

        LOAD_ERR_OTHER = -11,// paddle Lite backend的时候，如果MML的header和库不匹配，或者因为模型和Lite不匹配，以及其他一些未知原因加载失败的时候，返回该错误。
        
        RUN_ERR_PREDICT = -23, // iOS GPU backend 预测的时候，执行预测失败的时候，返回该错误。
        
        RUN_ERR_MACHINE_TYPE = -24, // MML Service load的时候传入错误的machine_type，或者iOS平台上使用了不支持GPU的差异化版本加载GPU service的时候，返回该错误。
        
        RUN_ERR_MACHINE_HANDLE = -25, // MML service 执行predict的时候，并没有找到backend（或load的时候创建backend失败）的时候，返回该错误

    };
```
