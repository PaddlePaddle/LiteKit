/*
 Copyright © 2020 Baidu, Inc. All Rights Reserved.

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
 to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#import "ViewController.h"
#import "ViewController+LiteKitCore_CPP.h"
#import "ViewController+LiteKitCore_OC.h"
#import "ViewController+View.h"

///litekit  header
#import "litekit_inference_api.h"
#import "paddle_lite_image_preprocess.h"



@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor lightGrayColor];
    
    NSBundle *testBundle = [NSBundle mainBundle];
    NSString *filePath = @"LiteKitCoreDemo.bundle";
    NSString *modelDir = [testBundle pathForResource:filePath ofType:nil];
    
    [self createViews];
    
    /// LiteKit  demo code
    [self LiteKit_CPP_Demo_GPU:modelDir];
    [self LiteKit_CPP_Demo_CPU:modelDir];

    /// LiteKit OC demo code
    [self LiteKit_OC_Demo_GPU:modelDir];
    [self LiteKit_OC_Demo_CPU:modelDir];
    
    // Do any additional setup after loading the view.
}

#pragma mark  - C++ Demo

NSUInteger n = 1, c = 3, h = 256, w = 256;
- (void *)LiteKit_CPP_Demo_GPU:(NSString *)modelDir {
    printf("\n>>>>>>>>>   CPP_Demo_GPU   <<<<<<<<<\n");
    printf("\ninput: \n");
    NSError *error = nil;
    NSString *inputName = [modelDir stringByAppendingPathComponent:@"input_1_3_256_256"];
    NSData *inputData = [NSData dataWithContentsOfFile:inputName options:0 error:&error];
    [self logBuffer:(float *)inputData.bytes length:inputData.length / sizeof(float) count:20];
    
    litekit_framework::LiteKitData input = {};
    input.autoRelease = false;
    input.rawDataShape = litekit_framework::LiteKitData::RawDataShape((int)n, (int)c, (int)h, (int)w);
    input.rawData = const_cast<void *>(inputData.bytes);
    input.dataLength = inputData.length;
   
    litekit_framework::LiteKitData output = {};
    output.autoRelease = false;
    
    std::shared_ptr<litekit_framework::LiteKitMachineService> service = [self loadLiteKitWithModelDir_GPU_CPP:modelDir];
    service->run(input, &output);
    printf("\noutput: \n");
    [self logBuffer:(float *)(output.rawData) length:output.dataLength count:20];
      
    printf("\n^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
    return output.rawData;
}

- (void)logBuffer:(float *)buffer length:(NSInteger)length count:(NSUInteger)count {
    NSInteger stride = length / count;
    if (stride == 0) {
        stride = 1;
    }
    for (int j = 0; j < length / stride; j++) {
        printf("%f ", buffer[j * stride]);
    }
    printf("\n");
}


- (void)LiteKit_CPP_Demo_CPU:(NSString *)modelDir {
    printf("\n>>>>>>>>>   CPP_Demo_GPU   <<<<<<<<<\n");
    /// shared ptr
    @autoreleasepool {
        // load
        std::shared_ptr<litekit_framework::LiteKitMachineService> service = [self loadLiteKitWithModelDir_Shared:modelDir];
        if (nullptr == service) {
            NSLog(@"LiteKit_Fail");
        } else {
            __block float *output = nullptr;
            
            NSLog(@"LiteKit_predict");
            output = [self predictWithMachine_shared:service];

            free(output);
        }
     }
    
    printf("\n\n=====================================\n\n");
    
    /// normal ptr
    @autoreleasepool {
        // load
        litekit_framework::LiteKitMachineService *service = [self loadLiteKitWithModelDir_LITE_MODEL_FROM_FILE:modelDir];
        if (nullptr == service) {
            NSLog(@"LiteKit_Fail");
        } else {
            __block float *output = nullptr;
            
            NSLog(@"LiteKit_predict");
            output = [self predictWithMachine:service];

            free(output);
            
            // release
            delete service;
        }
    }
    
    printf("\n^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
}

- (float *)predictWithMachine:(litekit_framework::LiteKitMachineService *)service {
    ///Demo Data
    litekit_framework::shape_t shape({1, 1, 224, 224});
    int64_t inputDemoDataSize = demo_shapeProduction(shape);
    float *inputDemoData = (float *)malloc(sizeof(float)*inputDemoDataSize);
    for (int i=0; i<inputDemoDataSize; i++) {
        inputDemoData[i] = 1.0f;
    }
    
    // 3. Prepare input data
    std::unique_ptr<litekit_framework::LiteKitData> inputData = service->getInputData(0);
    litekit_framework::LiteKitTensor *ainput = inputData->litekitTensor;

    ainput->Resize(shape);
    auto *data = ainput->mutable_data<float>();
    memcpy(data, inputDemoData, inputDemoDataSize*sizeof(float));
    
    printf("\ninput: \n");
    [self logBuffer:(float *)inputDemoData length:inputDemoDataSize count:20];
    
    // 4. Predict
    service->run();

    // 5. Get output
    std::unique_ptr<const litekit_framework::LiteKitData> outputData = service->getOutputData(0);
    const litekit_framework::LiteKitTensor *output_tensor = outputData->litekitTensor;

    int64_t outputDataSize = demo_shapeProduction(output_tensor->shape());
    int64_t dataSizex = outputData->dataLength;
    float *output = (float *)malloc(sizeof(float)*outputDataSize);
    memcpy(output, output_tensor->data<float>(), sizeof(float)*outputDataSize);
    
    std::vector<std::string> names = service->getOutputNames();

    if (names.size() > 1) {
        // 5. Get output
        std::unique_ptr<const litekit_framework::LiteKitData> outputData2 = service->getOutputData(1);
        const litekit_framework::LiteKitTensor *output_tensor2 = outputData2->litekitTensor;
        //    delete outputData;
        int64_t outputDataSize2 = demo_shapeProduction(output_tensor2->shape());
        int64_t dataSizex2 = outputData2->dataLength;
        float *output2 = (float *)malloc(sizeof(float)*outputDataSize2);
        memcpy(output2, output_tensor2->data<float>(), sizeof(float)*outputDataSize2);
        free(output2);
    }
    
    printf("\noutput: \n");
    [self logBuffer:(float *)output length:outputDataSize count:20];
    
    free(inputDemoData);
    
    return output;
}


- (float *)predictWithMachine_shared:(std::shared_ptr<litekit_framework::LiteKitMachineService>)service {
    ///Demo Data
    litekit_framework::shape_t shape({1, 1, 192, 192});
    int64_t inputDemoDataSize = demo_shapeProduction(shape);
    float *inputDemoData = (float *)malloc(sizeof(float)*inputDemoDataSize);
    for (int i=0; i<inputDemoDataSize; i++) {
        inputDemoData[i] = 1.0f;
    }
    
    // 3. Prepare input data
    std::unique_ptr<litekit_framework::LiteKitData> inputData = service->getInputData(0);
    litekit_framework::LiteKitTensor *ainput = inputData->litekitTensor;

    ainput->Resize(shape);
    auto *data = ainput->mutable_data<float>();
    memcpy(data, inputDemoData, inputDemoDataSize*sizeof(float));

    printf("\ninput: \n");
    [self logBuffer:(float *)inputDemoData length:inputDemoDataSize count:20];
    
    // 4. Predict
    service->run();

    // 5. Get output
    std::unique_ptr<const litekit_framework::LiteKitData> outputData = service->getOutputData(0);
    const litekit_framework::LiteKitTensor *output_tensor = outputData->litekitTensor;

    int64_t outputDataSize = demo_shapeProduction(output_tensor->shape());
    int64_t dataSizex = outputData->dataLength;
    float *output = (float *)malloc(sizeof(float)*outputDataSize);
    memcpy(output, output_tensor->data<float>(), sizeof(float)*outputDataSize);
    
    std::vector<std::string> names = service->getOutputNames();

    if (names.size() > 1) {
        // 5. Get output
        std::unique_ptr<const litekit_framework::LiteKitData> outputData2 = service->getOutputData(1);
        const litekit_framework::LiteKitTensor *output_tensor2 = outputData2->litekitTensor;
        //    delete outputData;
        int64_t outputDataSize2 = demo_shapeProduction(output_tensor2->shape());
        int64_t dataSizex2 = outputData2->dataLength;
        float *output2 = (float *)malloc(sizeof(float)*outputDataSize2);
        memcpy(output2, output_tensor2->data<float>(), sizeof(float)*outputDataSize2);
        free(output2);
    }
       
    printf("\noutput: \n");
    [self logBuffer:(float *)output length:outputDataSize count:20];
    
    free(inputDemoData);
    
    return output;
}


#pragma mark - utils
int64_t demo_shapeProduction(const litekit_framework::shape_t shape) {
  int64_t res = 1;
  for (auto i : shape) res *= i;
  return res;
}

#pragma mark - OC Demo

- (void)LiteKit_OC_Demo_GPU:(NSString *)modelDir {
    printf("\n>>>>>>>>>   OC_Demo_GPU   <<<<<<<<<\n");
    
    LiteKitBaseMachine *litekitMachine = [self loadLiteKitWithModelDir_GPU:modelDir];
    
    // 数据准备
    NSInteger n = kLiteKitInputBatch;
    NSInteger c = kLiteKitInputChannel;
    NSInteger h = kLiteKitInputHeight;
    NSInteger w = kLiteKitInputWidth;
    NSArray *dims = @[@(n), @(c), @(h), @(w)];
    float *image_data = (float *)malloc(sizeof(float)*w*h*c);
    for(int i=0; i<w*h; i++) {
        image_data[i] = 1;
    }
    LiteKitShapedData *shapeData = [[LiteKitShapedData alloc] initWithData:image_data dataSize:w*h*c dims:dims];
    LiteKitData *inputData = [[LiteKitData alloc] initWithData:shapeData type:TLiteKitDataTypeLiteKitShapedData];
    
    // run sync
    NSError *error = nil;
    LiteKitData *outputData = [litekitMachine predictWithInputData:inputData error:&error];
    
    NSLog(@"end\n outputData = %d, dims = %@\n", outputData.litekitShapedData.dataSize, outputData.litekitShapedData.dim);
    
    printf("\n^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
}

- (void)LiteKit_OC_Demo_CPU:(NSString *)modelDir {
    printf("\n>>>>>>>>>   OC_Demo_CPU   <<<<<<<<<\n");
    
    LiteKitBaseMachine *litekitMachine = [self loadLiteKitWithModelDir_CPU:modelDir];
    
    // 数据准备
    NSInteger n = kLiteKitInputBatch;
    NSInteger c = kLiteKitInputChannel;
    NSInteger h = kLiteKitInputHeight;
    NSInteger w = kLiteKitInputWidth;
    NSArray *dims = @[@(n), @(c), @(h), @(w)];
    float *image_data = (float *)malloc(sizeof(float)*w*h*c);
    for(int i=0; i<w*h; i++) {
        image_data[i] = 1;
    }
    LiteKitShapedData *shapeData = [[LiteKitShapedData alloc] initWithData:image_data dataSize:w*h*c dims:dims];
    LiteKitData *inputData = [[LiteKitData alloc] initWithData:shapeData type:TLiteKitDataTypeLiteKitShapedData];
    
    // run sync
    NSError *error = nil;
    LiteKitData *outputData = [litekitMachine predictWithInputData:inputData error:&error];
    
    NSLog(@"end\n outputData = %d, dims = %@\n", outputData.litekitShapedData.dataSize, outputData.litekitShapedData.dim);
    
    printf("\n^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
}

@end
