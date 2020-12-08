### 构建

- 统一环境

  - ndk版本: android-ndk-r16b

  - c++运行库: ANDROID_STL=c++_shared

- 下载[依赖库](https://gitee.com/paddlepaddle/LiteKit/tree/main/Android/LiteKitCoreNativeDepencency/paddle_api_light_bundled/0.0.9)

  - 放到以下路径中

  - src/main/jniLibs/arm64-v8a/libpaddle_api_light_bundled.a

  - src/main/jniLibs/armeabi-v7a/libpaddle_api_light_bundled.a

- 执行脚本

  - 在~/.bashrc里指定`ANDROID_CMAKE`和`ANDROID_NDK`变量

  - `sh build_android.sh`

- 输出产物

  - `./output/litekit_inference_api.h`
  - `./output/armeabi-v7a/liblitekit_framework.so`
  - `./output/arm64-v8a/liblitekit_framework.so`
