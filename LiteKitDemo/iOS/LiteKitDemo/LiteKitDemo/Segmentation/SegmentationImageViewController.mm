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


#import "SegmentationImageViewController.h"
#import <Accelerate/Accelerate.h>
#import <LiteKitPortraitSegmentation/LiteKitPortraitSegmentor.h>

#define Output_Width  192
#define Output_Height  192

@interface SegmentationImageViewController ()
@property (strong, nonatomic) UIImage *image;
@property (strong, nonatomic) UILabel *timeCost;
@property (nonatomic, strong) UIImageView *imageView;

@property (nonatomic, assign) CGSize presetSize;        //image大小
@property (nonatomic, assign) float *alpha_output;      //模型输出unit8 -> float
@property (nonatomic, assign) float *alpha_scale;       //放大到目标大小
@property (nonatomic, assign) uint8_t *rgb_output;      //融合后输出
@property (nonatomic, assign) uint8_t *rgba_capture;    //camera输出
@property (nonatomic, strong) LiteKitPortraitSegmentor *portraitSegmentor;

@end

@implementation SegmentationImageViewController

-(void)dealloc {
    if (_alpha_output) free(_alpha_output);
    if (_alpha_scale) free(_alpha_scale);
    if (_rgb_output) free(_rgb_output);
    if (_rgba_capture) free(_rgba_capture);
}

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor whiteColor];
    [self initData];
    [self initView];
    [self createInstance];
}

- (void)initData {
    self.image = [UIImage imageNamed:@"test_img.png"];
    self.presetSize = self.image.size;
    
    self.alpha_output = (float *)calloc(Output_Width * Output_Height, sizeof(float));
    self.alpha_scale = (float *)calloc(self.presetSize.width * self.presetSize.height * 4, sizeof(float));
    self.rgb_output = (uint8_t *)calloc(self.presetSize.width * self.presetSize.height * 4, sizeof(uint8_t));
    self.rgba_capture = (uint8_t *)calloc(self.presetSize.width * self.presetSize.height * 4, sizeof(uint8_t));
}

- (void)initView {
    // init navi
    self.navigationItem.rightBarButtonItems = @[
        [[UIBarButtonItem alloc] initWithTitle:@"开始识别"
                                         style:UIBarButtonItemStylePlain
                                        target:self
                                        action:@selector(imageDetectAction:)]
    ];
    
    CGFloat imgW = self.image.size.width;
    CGFloat imgH = self.image.size.height;
    CGFloat adjustH = imgH * self.view.frame.size.width / imgW;
    UIImageView *imageView = [[UIImageView alloc] initWithFrame:CGRectMake(0, [self naviBarHeight], self.view.frame.size.width, adjustH)];
    imageView.image = self.image;
    imageView.backgroundColor = [UIColor clearColor];
    imageView.contentMode = UIViewContentModeScaleAspectFit;
    [self.view addSubview:imageView];
    self.imageView = imageView;
    
    UILabel *label = [[UILabel alloc] initWithFrame:CGRectMake(12, [self naviBarHeight] + 8, 100, 30)];
    label.textColor = [UIColor redColor];
    [self.view addSubview:label];
    self.timeCost = label;
}

- (BOOL)isNeedCamera {
    return NO;
}

//横竖屏切换时：显示相机流的layer更新frame
- (void)updateViewLayout:(CGSize)screenSize  {
    [super updateViewLayout:screenSize];
    
    CGFloat imgW = self.image.size.width;
    CGFloat imgH = self.image.size.height;
    CGFloat adjustH = imgH * self.view.frame.size.width / imgW;
    self.imageView.frame = CGRectMake(0, [self naviBarHeight], screenSize.width, adjustH);
    [self.view bringSubviewToFront:self.imageView];
}

- (void)createInstance {
    NSError *error = nil;
    self.portraitSegmentor = [LiteKitPortraitSegmentor createPortraitSegmentorWithError:&error];
    if (!self.portraitSegmentor || error) {
        return;
    }
}

- (void)imageDetectAction:(id)sender {
    
    NSTimeInterval begin = [[NSDate date] timeIntervalSince1970];
    LiteKitPSData *output = (LiteKitPSData *)[self.portraitSegmentor inferWithImage:self.image error:nil];
    NSTimeInterval end = [[NSDate date] timeIntervalSince1970];
    
    uint8_t *alpahUnit8 = (uint8_t *)output->data;
    if (alpahUnit8 == nullptr) {
        return;
    }
    UIImage *mergeImage = [self mergeImage:self.image alpha:alpahUnit8];
    self.imageView.image = mergeImage;
    self.timeCost.text = [NSString stringWithFormat:@"%.2fms", (end - begin) * 1000];
}

- (UIImage *)mergeImage:(UIImage *)image alpha:(uint8_t *)alphaUint8 {
    //1、sampleBuffer转为image 再获取image对应的rgba数据
    {
        int width = (int)image.size.width;
        int height = (int)image.size.height;
        CGColorSpaceRef colorSpace = CGImageGetColorSpace(image.CGImage);
        CGContextRef contextRef = CGBitmapContextCreate(self.rgba_capture,
                                                        width,
                                                        height,
                                                        8,
                                                        width * 4,
                                                        colorSpace,
                                                        kCGImageAlphaNoneSkipLast | kCGBitmapByteOrderDefault);
        
        CGContextDrawImage(contextRef, CGRectMake(0, 0, width, height), image.CGImage);
        CGContextRelease(contextRef);
    }
    
    //放大模型输出到capture大小
    [self scaleToCaptureSize:alphaUint8];
    
    //利用放大后的alpha及capture_rgba数据进行融合
    return [self mergeCaptureWithWhiteColor];
}

- (void)scaleToCaptureSize:(uint8_t *)alphaUint8 {
    //uint8_t -> float 利用Accelerate库放大是需要使用float类型数据
    for (int i = 0; i < Output_Width * Output_Height; i++) {
        self.alpha_output[i] = alphaUint8[i] / 255.0;
    }
    
    //模型输出192*192 放大到目标区域
    int width = (int)self.presetSize.width;
    int height = (int)self.presetSize.height;
    
    vImage_Buffer sBuffer = {
        .data = self.alpha_output,
        .height = Output_Height,
        .width = Output_Width,
        .rowBytes = sizeof(float) * Output_Width,
    };
    
    vImage_Buffer dBuffer = {
        .data = self.alpha_scale,
        .height = static_cast<vImagePixelCount>(height),
        .width = static_cast<vImagePixelCount>(width),
        .rowBytes = sizeof(float) * width,
    };
    
    // 放大
    vImageScale_PlanarF(&sBuffer, &dBuffer, NULL, kvImageNoFlags);
}

- (UIImage *)mergeCaptureWithWhiteColor {
    int width = (int)self.presetSize.width;
    int height = (int)self.presetSize.height;
    int number = width * height;
    
    float *alpha = self.alpha_scale;
    uint8_t *input = self.rgba_capture;
    uint8_t *output = self.rgb_output;
    int x = arc4random() % 255;

    for (int i = 0; i < number; i++){
        if (*alpha >= 0.1) {
            output[0] = input[0];
            output[1] = input[1];
            output[2] = input[2];
        } else {
            output[0] = 0;
            output[1] = x;
            output[2] = 0;
        }
        output[3] = 255;
        
        alpha++;
        input += 4;
        output += 4;
    }
    
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGContextRef context = CGBitmapContextCreate(self.rgb_output,
                                                 width,
                                                 height,
                                                 8,
                                                 width * 4,
                                                 colorSpace,
                                                 kCGImageAlphaNoneSkipLast | kCGBitmapByteOrderDefault);
    CGImageRef cgImage = CGBitmapContextCreateImage(context);
    UIImage *resImage = [UIImage imageWithCGImage:cgImage];
    
    CGImageRelease(cgImage);
    CGColorSpaceRelease(colorSpace);
    CGContextRelease(context);
    
    return resImage;
}

- (float)naviBarHeight {
    return self.navigationController.navigationBar.frame.size.height + [[UIApplication sharedApplication] statusBarFrame].size.height;
}

@end
