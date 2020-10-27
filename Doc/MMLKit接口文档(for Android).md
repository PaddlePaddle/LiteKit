# MMLKit接口文档(for Android)

## 人像分割
人像分割的config定义MMLPortraitSegmentationConfig
```
/**
 * @desc 设置config中的模型路径
 * @param path 模型路径
 */
public void setModelUrl(String path) 
```

初始化人像分割的MMLPortraitSegmentation
```
/**
 * @desc 对MMLPortraitSegmentation进行初始化
 * @param config 初始化配置config
 */
public MMLPortraitSegmentation(MMLPortraitSegmentationConfig config)
```

人像分割MMLPortraitSegmentation的预测接口
```
/**
 * @desc 通过byte数据进行预测
 * @param img_data 待预测的数据
 * @param imgWidth 图像Width
 * @param imgHeight 图像Height
 * @return int[] 预测结果
 */
public int[] predictor(byte[] img_data, int width, int height)

/**
 * @desc 通过bitmap数据进行预测
 * @param img_data 待预测的bitmap数据
 * @return int[] 预测结果
 */
public int[] predictor(Bitmap img_data)
```

释放人像分割资源
```

/**
 * @desc 对人像分割MMLPortraitSegmentation进行释放
 */
public void release()
```


## 手势识别
对手势识别的HandGestureDetector进行初始化
```
example：HandGestureDetector.init(this, modelPath)

其中：
/**
 * @desc 对HandGestureDetector进行初始化
 * @param context 上下文context
 * @param modelFilePath 模型文件的地址（含模型名字）
 */
public static boolean init(Context context, String modelFilePath)
```

手势识别的预测接口
```
example：HandGestureDetector.detect(scaleImage);

其中：
/**
 * @desc 通过bitmap进行预测
 * @param image 待预测的内容的bitmap
 * @return HandGestureDetectResult 预测结果
 */
public static HandGestureDetectResult detect(Bitmap image);

/**
 * @desc 通过byte数据进行预测
 * @param data 待预测的数据
 * @param imgWidth 图像Width
 * @param imgHeight 图像Height
 * @return HandGestureDetectResult 预测结果
 */
public static HandGestureDetectResult detect(byte[] data, int imgWidth, int imgHeight) 
```

预测返回结果结构HandGestureDetectResult
```
public class HandGestureDetectResult {
    public RectF handBoxRect; // 识别出的手势的位置的的rect
    public PointF fingerPoint; // 手势中手指的位置的point
    public HandGestureDetectResult.GestureLabel label; // 识别出手势的类型
    public float confidence; // 置信度

    public static enum GestureLabel {
        Hand, //手
        Five, // 五指手势
        Victory, // 胜利V手势
        Fist, // 握拳手势
        One, // 指/1手势
        OK; // OK手势

        private GestureLabel() {
        }
    }
}
```

使用完成之后，对手势识别进行释放
```
example:HandGestureDetector.release();

其中
/**
 * @desc 对HandGestureDetector中需要释放的资源进行释放
 */
public static boolean release()
```

## 视频超分
视频超分使用的是YUV数据，以下为YUV数据结构定义，用于预测的input和output

对视频超分SrBridge进行初始化方法
```
example：SrBridge.nativeInitSr(FileManager.sr32(this));

其中：
/**
 * @desc 对SrBridge进行初始化
 * @param modelFilePath 模型文件的地址（含模型名字）
 * @return SrBridge的 handler，执行超分的时候需要使用
 */
public static native long nativeInitSr(String modelFilePath);
```

执行视频超分接口
```

/**
 * @desc 对bitmap执行超分
 * @param handle SrBridge的handler
 * @param in input数据bitmap
 * @param scale scale倍数
 * @return Bitmap output数据bitmap
 */
public static native Bitmap nativePredictBitmap(long handle, Bitmap in, float scale);


/**
 * @desc 对bitmap执行超分
 * @param handle SrBridge的handler
 * @param in rgbabytearray数据rgba的数据数组
 * @param height input的height
 * @param width input的width
 * @param scale scale倍数
 * @return byte[] output数据
 */
public static native byte[] nativePredictRGBA(long handle, byte[] rgbabytearray, int height, int width, float scale);
    
```


释放超分SrBridge中JNI资源
```
/**
 * @desc 对SrBridge进行释放
 */
public static native void nativeReleaseSrSdk(long handle);
```