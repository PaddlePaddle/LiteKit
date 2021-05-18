//
//  ImageViewController.m
//  LiteKitCoreDemo
//
//  Created by Wang,Zhiyong on 2021/5/18.
//  Copyright © 2021 Baidu. All rights reserved.
//

#import "ImageViewController.h"

// 引入openCV头文件
#import <opencv2/opencv.hpp>

#include <fstream>
/// 引入该头文件可以使用ARM NEON加速
#include <arm_neon.h>

///litekit  header
#import <LiteKitCore/LiteKitPaddleConfig.h>
#import <LiteKitCore/LiteKitMachineConfig.h>
#import <LiteKitCore/LiteKitMachineService.h>
#import <LiteKitCore/LiteKitDataProcess.h>
#import <LiteKitCore/LiteKitLogger.h>
#import <LiteKitCore/LiteKitCore.h>

#define INPUT_BATCH      1
#define INPUT_CHANNEL    3
#define INPUT_HEIGHT     224
#define INPUT_WIDTH      224
#define UNKNOWN_LABEL    "unknown"

@interface ImageViewController ()

@end

@implementation ImageViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor whiteColor];
    self.navigationItem.rightBarButtonItems = nil;
    [self liteKitDemo];
}

-(void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    self.navigationController.navigationBar.hidden = NO;
}
    
- (void)liteKitDemo {
    // paths
    NSBundle *testBundle = [NSBundle mainBundle];
    NSString *filePath = @"LiteKitCoreDemo.bundle";
    NSString *modelDir = [testBundle pathForResource:filePath ofType:nil];
    
    NSString *labelPath = [modelDir stringByAppendingPathComponent:@"cat_labels.txt"];
    NSString *modelPath = [modelDir stringByAppendingPathComponent:@"cat_mobilenetv2.nb"];
    NSString *imagePath = [modelDir stringByAppendingPathComponent:@"cat.jpeg"];
    
    /* 1. load machine */
    LiteKitBaseMachine *litekitMachine = [self loadLiteKit:modelPath];
    
    /* 2. create input */
    // input images
    UIImage *catImage = [UIImage imageWithContentsOfFile:imagePath];
    cv::Mat img = [LiteKitDataProcess litekit_CVMatFromUIImage:catImage];
    float *input = new float[INPUT_BATCH * INPUT_HEIGHT * INPUT_WIDTH * INPUT_CHANNEL];
    Preprocess(img, input);
    // LiteKitShapedData
    LiteKitShapedData *shapeData =
    [[LiteKitShapedData alloc] initWithData:input
                                   dataSize:INPUT_BATCH * INPUT_HEIGHT * INPUT_WIDTH * INPUT_CHANNEL
                                       dims:@[@(INPUT_BATCH), @(INPUT_CHANNEL), @(INPUT_HEIGHT), @(INPUT_WIDTH)]];
    // LiteKitData
    LiteKitData *inputData = [[LiteKitData alloc] initWithData:shapeData type:TLiteKitDataTypeLiteKitShapedData];
    
    /* 3. Predict */
    NSError *error = nil;
    LiteKitData *outputData = [litekitMachine predictWithInputData:inputData error:&error];
    
    /* 4. get output */
    float *output = outputData.litekitShapedData.data;
    NSArray<NSNumber *> *out_shape = outputData.litekitShapedData.dim;
    int shape_production = (int)(out_shape[0].intValue * out_shape[1].intValue * out_shape[2].intValue * out_shape[3].intValue);
    std::vector<std::string> labels = LoadLabels(labelPath.UTF8String);
    std::string result_label = Postprocess(output, shape_production, labels);
    
    printf("=================\n");
    printf("预测结果是：%s\n", result_label.c_str());
    printf("=================\n");
    
    
    UIImageView *imageView = [[UIImageView alloc] initWithImage:catImage];
    CGRect frame = imageView.frame;
    frame.origin.y = 70;
    imageView.frame = frame;
    [self.view addSubview:imageView];

    frame.origin.y = frame.origin.y + frame.size.height;
    frame.size.height = 50;
    UILabel *label = [[UILabel alloc] initWithFrame:frame];
    label.text = [NSString stringWithUTF8String:result_label.c_str()];
    [self.view addSubview:label];
}


- (LiteKitBaseMachine *)loadLiteKit:(NSString *)modelPath {
    // construct litekit config
    LiteKitMachineConfig *machineConfig = [[LiteKitMachineConfig alloc] init];
    machineConfig.modelPath = modelPath;
    machineConfig.machineType = LiteKitPaddleCPU;

    // 加载模型
    NSError *aError = nil;
    LiteKitMachineService *service = [[LiteKitMachineService alloc] init];
    LiteKitBaseMachine *litekitMachine = [service loadMachineWithConfig:machineConfig error:&aError];
    
    return litekitMachine;
}


void Preprocess(cv::Mat &mat, float *buffer) {
    cv::Mat img;
    
    // convert to RGB color space
    if (mat.channels() == 4) {
        cv::cvtColor(mat, img, CV_RGBA2RGB);
    } else {
        img = mat;
    }
    
    // resize to 224 * 224
    cv::resize(img, img, cv::Size(INPUT_WIDTH, INPUT_HEIGHT), 0.f, 0.f);

    // normalize
    img.convertTo(img, CV_32FC3, 1 / 255.f);

    // change layout
    static std::vector<float> scale = {0.229f, 0.224f, 0.225f};
    static std::vector<float> mean = {0.485f, 0.456f, 0.406f};
//    const float* dimg = reinterpret_cast<const float*>(img.data);
//    neon_mean_scale(dimg, buffer, INPUT_WIDTH * INPUT_HEIGHT, mean, scale);
    for (int i = 0; i < 3; i++) {
        cv::Mat channel_mat = cv::Mat(INPUT_WIDTH, INPUT_HEIGHT, CV_32FC1, buffer + i * INPUT_WIDTH * INPUT_HEIGHT);
        cv::extractChannel(img, channel_mat, i);

        // apply scale and mean
        int start_index = i * INPUT_WIDTH * INPUT_HEIGHT;
        int end_index = (i + 1) * INPUT_WIDTH * INPUT_HEIGHT;
        for (int j = start_index; j < end_index; j++) {
            buffer[j]  = (buffer[j] - mean[i]) / scale[i];
        }
    }
}

std::string Postprocess(const float *scores, int size, std::vector<std::string> labels) {
    float max_value = -1.0;
    int max_index = -1;
    for (int index = 0; index < size; index++) {
        if (max_value < scores[index]) {
            max_value = scores[index];
            max_index = index;
        }
    }
   
    if (max_index >= 0 && max_index < labels.size()) {
        return labels[max_index];
    }
    return UNKNOWN_LABEL;
}

std::vector<std::string> LoadLabels(const std::string &label_path) {
    std::vector<std::string> labels;
    std::ifstream file;
    file.open(label_path);
    if (file.is_open()) {
        while (!file.eof()) {
            std::string line;
            getline(file, line);
            labels.push_back(line);
        }
        file.close();
    }
    return labels;
}

@end
