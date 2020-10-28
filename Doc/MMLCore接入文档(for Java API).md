# MMLCore接入文档 (for Java API)


## 一、编译
### 1. 构建环境
MML Java API产物依赖及版本：
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
 - 根据文档[MMLCore/MML/C++](MMLCore/MML/C%2B%2B/README.md), 编译生成`libmml_framework.so`
 - 根据文档[MMLCore/MML/Android](MMLCore/MML/Android/README.md), 编译生成`mmlcore-debug.aar`

## 二、集成
### 1. 导入SDK
将MML Java的可执行文件`libmml_framework.so`和`mmlcore.aar`放在工程目录app/libs/下


### 2. 添加第三方依赖
无

### 3. 添加系统framework
无

### 4. 配置环境
无

### 5. 资源文件
无

## 三、使用
   MML Java API执行推理的时候。
   <br>
   首先，需要确定模型、所使用的backend、模型和backend是否匹配、以及其他config参数，并创建config。
   <br>
   然后，通过config 对Service进行load，由Service创建的machine直接管理预测backend，用户仅需要持有/释放machine，不需要对backend的实例进行直接管理。
   <br>
   machine的预测接口input和output数据都是MMLData类型，MMLData是MML Java API对数据格式的封装，提供设置input数据和读取output数据的能力。
   <br>
   machine使用完成后，需要手动执行释放操作，以释放machine持有的相关资源。
    

### 1) 引入头文件
```
// import MML Java header
import com.baidu.mmlcore.MMLBaseMachine;
import com.baidu.mmlcore.MMLData;
import com.baidu.mmlcore.MMLMachineConfig;
import com.baidu.mmlcore.MMLMachineService;
import com.baidu.mmlcore.MMLPaddleConfig;
import com.baidu.mmlcore.MMLPaddleLiteConfig;
```

### 2) 创建Config （含模型转换）
```
// 创建MML Java Config
MMLMachineConfig machineConfig = new MMLMachineConfig();
MMLPaddleConfig paddleConfig = new MMLPaddleConfig();
paddleConfig.liteConfig = new MMLPaddleLiteConfig();
machineConfig.modelPath = modelPath();
machineConfig.machineType = MMLMachineConfig.MachineType.MMLPaddleLite;
machineConfig.engineConifg = paddleConfig;
```

### 3) 创建Machine
```
//创建machine
MMLBaseMachine machine = MMLMachineService.loadMachineWithConfig(machineConfig);
```
### 4) 前处理
```
// 进行需要的前处理
float *inputData;// example input数据
```

### 5) 创建Input
```
// create input data
ArrayList<MMLData> input = new ArrayList<>();
MMLData data = new MMLData(inputData, modelInputBatchSize, modelInputChannel, modelInputHeight, modelInputWidth, 0); // inputData为demo input data
input.add(data);
```

### 6) Machine执行predict
```
// 执行预测
ArrayList<MMLData> output = machine.predictWithInputData(input);
```

### 7) 读取output
```
// get output
float[] result = postprocess(output.get(0).output.fetchFloatData(), output.get(1).output.fetchFloatData(), output.get(2).output.fetchFloatData(),
                output.get(3).output.fetchFloatData(), output.get(4).output.fetchFloatData(), image.getWidth(), image.getHeight());
        
```
 
### 8) 后处理
```
//读取output数据后，进行需要的后处理
float *result = output.get(0).output.fetchFloatData()// example output data
```

### 9) 释放Machine
```
// 释放machine相关资源
machine.releaseMachine();
```

## 常见错误说明
```
-
```
