# MMLKit接入文档(for Android)
## 导入SDK
|能力 |对应SDK | 
|---|---|
| 人像分割| portraitsegmentation-0.0.9.aar | 
| 手势识别| gesturerecognize-0.0.9.aar | 
| 视频超分| superresolution-0.0.9.aar | 


## 添加第三方依赖
|依赖 | 版本| 
|---|---|
|mmlkitdependency-0.0.9.aar| 0.0.9| 
|thirdpartydependency-0.0.9.aar| 0.0.9|


## 添加系统framework
无

## 配置环境
无

## 资源文件
将模型文件放置到工程适当位置，创建能力实例的时候需要读取模型文件位置。

## 使用方法
### 人像分割
引入头文件
```
import com.baidu.mmlkit.portraitsegmentationsdk.MMLPortraitSegmentation;
import com.baidu.mmlkit.portraitsegmentationsdk.MMLPortraitSegmentationConfig;
```

创建Config（模型地址）、创建Predictor
```
MMLPortraitSegmentationConfig config = new MMLPortraitSegmentationConfig();
config.setModelUrl(modelPath); // modelPath为模型文件路径（含模型文件名称）
portraitSegmentation = new MMLPortraitSegmentation(config);
```

执行预测
```
portraitSegmentation.predictor(inputData, rgbaImage.getWidth(), rgbaImage.getHeight());
```

释放Predictor
```
portraitSegmentation.release();
```
### 手势识别
引入头文件
```
import com.baidu.gesturelibrary.HandGestureDetectResult;
import com.baidu.gesturelibrary.HandGestureDetector;
```

创建Predictor
```
HandGestureDetector.init(this, modelPath) // 其中modelPath为模型文件路径（含模型文件名字）
```

执行Predict、获取Output
```
HandGestureDetectResult result = HandGestureDetector.detect(imageData);
```

释放Predictor
```
HandGestureDetector.release();
```

### 视频超分
引入头文件
```
import com.baidu.sr.SrBridge;
```

创建Config（模型地址）、创建Predictor
```
Long handle = SrBridge.nativeInitSr(modelPath); // 其中modelPath为模型文件路径（含模型文件名字）
```

执行Predict、获取Output
```
Bitmap lowBitmap; // input待超分bitmap
int byteCount = lowBitmap.getByteCount();
ByteBuffer buf  = ByteBuffer.allocate(byteCount);
lowBitmap.copyPixelsToBuffer(buf);
byte[] rgba = buf.array();
byte[] targetRGBA = SrBridge.nativePredictRGBA(
                handle,      // 创建的predictorhandler
                rgba,        // 待超分rgba数据
                lowBitmap.getHeight(),
                lowBitmap.getWidth(),
                scale         // scale 倍数
        );

or

 Bitmap bitmap2 = SrBridge.nativePredictBitmap(
                    handle,       // 创建的predictorhandler
                    lowBitmap,    // 待超分bitmap
                    scale         // scale 倍数
            );
```
释放Predictor
```
// release SR machine
SrBridge.nativeReleaseSrSdk(handle);
```
