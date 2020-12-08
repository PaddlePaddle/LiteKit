### 构建

- 统一环境

  - ndk版本: android-ndk-r16b
- c++运行库: ANDROID_STL=c++_shared
  
- 编译依赖liblitekit_framework.so

  - 参考[这里](../C++/README.md), 然后将它们放到以下路径中

  - litekitcore/src/main/cpp/libs/arm64-v8a/liblitekit_framework.so

  - litekitcore/src/main/cpp/libs/armeabi-v7a/liblitekit_framework.so

- 编译

  - 执行android studio的按钮Build / Make Project编译

- 输出产物

  - `litekitcore/build/outputs/aar/litekitcore-debug.aar`
  - `litekitcore/build/outputs/aar/litekitcore-release.aar`