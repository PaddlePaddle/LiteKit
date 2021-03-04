### 构建

- 统一环境
- cmake 3.15.0

- 下载依赖库：
[PaddleCPU](https://gitee.com/paddlepaddle/LiteKit/tree/main/iOS/LiteKitCoreDependency/PaddleCPU/2.8.0rc/libpaddle_api_light_bundled.a)
放到以下目录：
    - `./LiteKitCore/LiteKitCore/C++/src/main/iOSLibs/paddle_api_light_bundled/libpaddle_api_light_bundled.a`

- 执行脚本

  - `cd /LiteKitCore/LiteKitCore/C++/build-ios`
  - `sh product_build.sh`

- 输出产物

  - `./output/product/release/include/litekit_inference_api.h`
  - `./output/product/release/liblitekit_framework_ios.a`
