### 构建

- 统一环境

  - ndk版本: android-ndk-r16b
- c++运行库: ANDROID_STL=c++_shared
  
- 编译依赖libmml_framework.so

  - 参考[这里](../C++/README.md), 然后将它们放到以下路径中

  - mmlcore/src/main/cpp/libs/arm64-v8a/libmml_framework.so

  - mmlcore/src/main/cpp/libs/armeabi-v7a/libmml_framework.so

- 编译

  - 执行android studio的按钮Build / Make Project编译

- 输出产物

  - `mmlcore/build/outputs/aar/mmlcore-debug.aar`
  - `mmlcore/build/outputs/aar/mmlcore-release.aar`