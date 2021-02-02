# LiteKitCore接口文档(for Java API)
## LiteKitBaseMachine
Machine的基类 
具体实现类为LiteKitPaddleLiteMachine

```java
/**
 * Predict with input data
 *
 * */
public abstract ArrayList<LiteKitData> predictWithInputData(ArrayList<LiteKitData> aInputData);

/**
 * Run the Machine.
 *
 * */
public native void run(long nativeMachineHandler);

/**
 * Delete C++ Machine object pointed by the input pointer, which is
 * presented by a long value.
 *
 */
public native void release(long nativeMachineHandler);

```

## LiteKitMachineConfig

LiteKitMachineService通过加载config创建basemachine
```java
/**
 * model file path
 */
public String modelPath;

public enum MachineType {
    LiteKitPaddleLite(2);

    private MachineType(int value) {
        this.value = value;
    }
    public final int value;
}

/**
 * machine type
 */
public MachineType machineType;

/**
 * machine_config
 */
public LiteKitInferenceEngineConfig engineConifg;
```

## LiteKitMachineService
根据config的配置创建对应的machine

```java
/// async
/// @param aConfig LiteKit配置
public static LiteKitBaseMachine loadMachineWithConfig(LiteKitMachineConfig aConfig);

```
## 封装的LiteKit数据类型
LiteKitTensorShape为LiteKitData中input & output的shape
```java
public class LiteKitTensorShape {
    public int inputBatch;
    public int inputChannel;
    public int inputHeight;
    public int inputWidth;

    public LiteKitTensorShape(int batch, int channel, int height, int width) {
        inputBatch = batch;
        inputChannel = channel;
        inputHeight = height;
        inputWidth = width;
    }
}
```


## LiteKitData
input & output 数据结构定义

1. 枚举定义
```java
/**
 * LiteKitTensor数据 数据类型定义
 */
 public enum Type {
     kFLOAT,
     kBYTE,
     kINT,
     kLONG
 };
```

LiteKitData的定义

```java
/**
 * LiteKitData定义
 */
/// 构造函数
public LiteKitData(float[] in, int batch, int channel, int height, int width, int id) ;
public LiteKitData(byte[] in, int batch, int channel, int height, int width, int id);
public LiteKitData(int[] in, int batch, int channel, int height, int width, int id);
public LiteKitData(long[] in, int batch, int channel, int height, int width, int id);

 /**
 * 数据相关属性的getter and setter
 * dataType、dataTensor等的getter和setter方法
 */
public Type getType();
public LiteKitTensorShape getShape();
public int getInputGraphId();
public void setType(Type type);
public void setShape(LiteKitTensorShape shape);
public void setInputGraphId(int inputGraphId);

/**
 * 数据getter and setter
 * 设置input数据和获取output数据
 */
public float[] getFloatData();
public byte[] getByteData();
public int[] getIntData();
public long[] getLongData() ;
public void setFloatData(float[] floatData);
public void setByteData(byte[] byteData);
public void setIntData(int[] intData);
public void setLongData(long[] longData);

```
