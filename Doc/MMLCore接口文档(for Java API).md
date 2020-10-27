# MMLCore接口文档(for Java API)
## MMLBaseMachine
Machine的基类 
具体实现类为MMLPaddleLiteMachine

```
/**
 * Predict with input data
 *
 **/
public abstract ArrayList<MMLData> predictWithInputData(ArrayList<MMLData> aInputData);


```

## MMLMachineConfig

MMLMachineService通过加载config创建basemachine
```
 /**
  * 模型文件路径
  */
public String modelPath;

public enum MachineType {
    MMLPaddleLite(2);

    private MachineType(int value) {
        this.value = value;
    }
    public final int value;
}

/**
 * 预测引擎类型
 */
public MachineType machineType;

/**
 * machine_config
 */
public MMLInferenceEngineConfig engineConifg;
```

## MMLMachineService
根据config的配置创建对应的machine

```
/// async
/// @param aConfig MML配置
public static MMLBaseMachine loadMachineWithConfig(MMLMachineConfig aConfig) 

```
## 封装的MML数据类型
MMLTensorShape为MMLData中input & output的shape
```
public class MMLTensorShape {
    public int inputBatch;
    public int inputChannel;
    public int inputHeight;
    public int inputWidth;

    public MMLTensorShape(int batch, int channel, int height, int width) {
        inputBatch = batch;
        inputChannel = channel;
        inputHeight = height;
        inputWidth = width;
    }
}
```


## MMLData
input & output 数据结构定义

1. 枚举定义
```
/**
 * MMLTensor数据 数据类型定义
 */
public enum Type {
    kFLOAT, // float
    kBYTE,  // Byte
    kINT,   // int
    kLONG   // long
};
```

MMLData的定义
```

/**
 * MMLData定义
 */
/// 构造函数
public MMLData(float[] in, int batch, int channel, int height, int width, int id) ;
public MMLData(byte[] in, int batch, int channel, int height, int width, int id);
public MMLData(int[] in, int batch, int channel, int height, int width, int id);
public MMLData(long[] in, int batch, int channel, int height, int width, int id);

 /**
 * 数据相关属性的getter and setter
 * dataType、dataTensor等的getter和setter方法
 */
public Type getType();
public MMLTensorShape getShape();
public int getInputGraphId();
public void setType(Type type);
public void setShape(MMLTensorShape shape);
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
