### 构建

- 统一环境

  - ndk版本: android-ndk-r16b

  - c++运行库: ANDROID_STL=c++_shared

- 下载[依赖库](http://agroup.baidu.com/MML/file/view/705639)

  - 放到以下路径中

  - src/main/jniLibs/arm64-v8a/libpaddle_api_light_bundled.a

  - src/main/jniLibs/armeabi-v7a/libpaddle_api_light_bundled.a

- 执行脚本

  - 在~/.bashrc里指定`ANDROID_CMAKE`和`ANDROID_NDK`变量

  - `sh build_android.sh`

- 输出产物

  - `./output/mml_inference_api.h`
  - `./output/armeabi-v7a/libmml_framework.so`
  - `./output/arm64-v8a/libmml_framework.so`