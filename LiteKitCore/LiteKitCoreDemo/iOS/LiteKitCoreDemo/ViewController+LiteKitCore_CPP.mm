// Copyright (c) 2019 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#import "ViewController+LiteKitCore_CPP.h"

///stc c++
#include <fstream>
#include <sstream>


@implementation ViewController(LiteKitCore_CPP)

- (litekit_framework::LiteKitMachineService *)loadLiteKitWithModelDir_LITE_MODEL_FROM_BUFFER:(NSString *)modelDir {
    /// init
    litekit_framework::LiteKitMachineService *service = new litekit_framework::LiteKitMachineService();

    litekit_framework::LiteKitConfig config;
    config.machine_type = litekit_framework::LiteKitConfig::MachineType::PaddleLite;

    litekit_framework::LiteKitConfig::PaddleLiteConfig paddle_config;
    paddle_config.powermode = litekit_framework::LiteKitConfig::PaddleLiteConfig::PaddleLitePowerMode::LITE_POWER_NO_BIND;
    paddle_config.threads = 1;
    
    
    /*********************************************************************************/
    /// fill paddle_config
    paddle_config.model_type = litekit_framework::LiteKitConfig::PaddleLiteConfig::LITE_MODEL_FROM_BUFFER;

    ///demo_1
    NSString *fileDir = [modelDir stringByAppendingPathComponent:@"mobilenet_v1_opt.nb"];
    FILE *fileFILE = fopen([fileDir UTF8String], "r");
    size_t sizeF2 = get_file_size(fileFILE);
    char *fileC = (char *)malloc(sizeof(char)*sizeF2);
    fread(fileC, 1, sizeF2, fileFILE);
    paddle_config.model.model_from_buffer.data = fileC;
    paddle_config.model.model_from_buffer.size = sizeF2;
//    ///demo_2
//    NSString *fileDir = [modelDir stringByAppendingPathComponent:@"human_float_opt.nb"];
//    auto model_file = fileDir.UTF8String;
//    std::string model_buffer = ReadFile(model_file);
//    paddle_config.model.model_from_buffer.data = (char *)model_buffer.c_str();
//    paddle_config.model.model_from_buffer.size = model_buffer.length();
    /**^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
    
    
    /// load
    config.machine_config.paddle_lite_config = paddle_config;
    int result = service->load(config);
    NSLog(@"result = %d",result);
    if (result != 0) {
        delete service;
        service = nullptr;
    }
    return service;
}


- (std::shared_ptr<litekit_framework::LiteKitMachineService>)loadLiteKitWithModelDir_Shared:(NSString *)modelDir {
    /// init

    litekit_framework::LiteKitConfig config;
    config.machine_type = litekit_framework::LiteKitConfig::MachineType::PaddleLite;

    litekit_framework::LiteKitConfig::PaddleLiteConfig paddle_config;
    paddle_config.powermode = litekit_framework::LiteKitConfig::PaddleLiteConfig::PaddleLitePowerMode::LITE_POWER_NO_BIND;
    paddle_config.threads = 1;
    
    
    /*********************************************************************************/
    paddle_config.model_type = litekit_framework::LiteKitConfig::PaddleLiteConfig::LITE_MODEL_FROM_FILE;
    
    NSString *fileDir = [modelDir stringByAppendingPathComponent:@"mobilenet_v1_opt.nb"];
    paddle_config.model.model_from_file.data = fileDir.UTF8String;
    paddle_config.model.model_from_file.size = fileDir.length;
    /**^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
    
    
    /// load
    config.machine_config.paddle_lite_config = paddle_config;
    
    std::shared_ptr<litekit_framework::LiteKitMachineService> service = litekit_framework::CreateLiteKitMachineService(config);
    
    return service;
}


- (litekit_framework::LiteKitMachineService *)loadLiteKitWithModelDir_LITE_MODEL_FROM_FILE:(NSString *)modelDir {
    /// init
    litekit_framework::LiteKitMachineService *service = new litekit_framework::LiteKitMachineService();

    litekit_framework::LiteKitConfig config;
    config.machine_type = litekit_framework::LiteKitConfig::MachineType::PaddleLite;

    litekit_framework::LiteKitConfig::PaddleLiteConfig paddle_config;
    paddle_config.powermode = litekit_framework::LiteKitConfig::PaddleLiteConfig::PaddleLitePowerMode::LITE_POWER_NO_BIND;
    paddle_config.threads = 1;
    
    
    /*********************************************************************************/
    /// fill paddle_config
    paddle_config.model_type = litekit_framework::LiteKitConfig::PaddleLiteConfig::LITE_MODEL_FROM_FILE;
    
    NSString *fileDir = [modelDir stringByAppendingPathComponent:@"mobilenet_v1_opt.nb"];
    paddle_config.model.model_from_file.data = fileDir.UTF8String;
    paddle_config.model.model_from_file.size = fileDir.length;
    /**^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
    
    
    /// load
    config.machine_config.paddle_lite_config = paddle_config;
    int result = service->load(config);
    NSLog(@"result = %d",result);
    if (result != 0) {
        delete service;
        service = nullptr;
    }
    return service;
}

- (std::shared_ptr<litekit_framework::LiteKitMachineService>)loadLiteKitWithModelDir_GPU_CPP:(NSString *)modelDir {
    NSString *gpuDir= [modelDir stringByAppendingPathComponent:@"mobilenetv2"];
    litekit_framework::LiteKitConfig config;
    config.machine_type = litekit_framework::LiteKitConfig::MachineType::PaddleiOSGPU;
    /*********************************************************************************/
    config.model_file_name = "model.mlm";
    config.param_file_name = "params.mlm";
    config.modelUrl = [gpuDir UTF8String]; // mv6s是多输出模型
    /**^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/
    litekit_framework::LiteKitConfig::PaddleiOSGPUConfig paddle_gpu_config;
    config.machine_config.paddle_ios_gpu_config = paddle_gpu_config;
    
    std::shared_ptr<litekit_framework::LiteKitMachineService> service = litekit_framework::CreateLiteKitMachineService(config);
    return service;
}

#pragma mark - utils
static std::string ReadFile(const std::string& filename) {
    std::ifstream ifile(filename.c_str());
    if (!ifile.is_open()) {
        NSLog(@"Open file: [ %s ] failed.", filename.c_str());
    }
    std::ostringstream buf;
    char ch;
    while (buf && ifile.get(ch)) buf.put(ch);
    ifile.close();

    return buf.str();
}

long get_file_size(FILE * file_handle) {
    //获取当前读取文件的位置 进行保存
    long current_read_position = ftell( file_handle );
    long file_size;
    fseek( file_handle, 0, SEEK_END );
    //获取文件的大小
    file_size=ftell( file_handle );
    //恢复文件原来读取的位置
    fseek( file_handle, current_read_position, SEEK_SET );
    return file_size;
}

@end
