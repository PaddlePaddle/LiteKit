### 构建

- 统一环境
- cmake 3.15.0

- 下载[依赖库](https://gitee.com/paddlepaddle/LiteKit/tree/main/iOS/LiteKitCoreNativeDepencency/0.0.9)

  - 放到以下路径中
  - LiteKitCore/LiteKitCore/C++/src/main/iOSLibs/

- 执行脚本

  - `cd /LiteKitCore/LiteKitCore/C++/build-ios`
  - `sh product_build.sh`

- 输出产物

  - `./output/product/release/include/litekit_inference_api.h`
  - `./output/product/release/liblitekit_framework_ios.a`
