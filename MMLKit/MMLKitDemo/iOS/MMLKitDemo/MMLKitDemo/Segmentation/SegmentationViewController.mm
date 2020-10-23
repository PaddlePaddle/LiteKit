//
//  SegmentationViewController.m
//  MMLKitDemo
//
//  Created by Baidu on 2020/7/2.
//  Copyright © 2020 Baidu Co.,Ltd. . All rights reserved.
//

#import "SegmentationViewController.h"
#import "SegmentationImageViewController.h"
#import <Accelerate/Accelerate.h>
#import <MMLPortraitSegmentation/MMLPortraitSegmentor.h>

#define Output_Width  192
#define Output_Height  192

@interface SegmentationViewController ()
@property (strong, nonatomic) UILabel *timeCost;
@property (nonatomic, strong) UIImageView *imageView;

@property (nonatomic, assign) CGSize presetSize;        //camera输出大小
@property (nonatomic, assign) float *alpha_output;      //模型输出unit8 -> float
@property (nonatomic, assign) float *alpha_scale;       //放大到目标大小
@property (nonatomic, assign) uint8_t *rgb_output;      //融合后输出
@property (nonatomic, assign) uint8_t *rgba_capture;    //camera输出
@property (nonatomic, strong) MMLPortraitSegmentor *portraitSegmentor;

@end

@implementation SegmentationViewController

-(void)dealloc {
    if (_alpha_output) free(_alpha_output);
    if (_alpha_scale) free(_alpha_scale);
    if (_rgb_output) free(_rgb_output);
    if (_rgba_capture) free(_rgba_capture);
}

- (void)viewDidLoad {
    [super viewDidLoad];
    [self initData];
    [self initView];
    [self createInstance];
}

- (void)initData {
    self.presetSize = [self captureSessionPresetSize];
    
    self.alpha_output = (float *)calloc(Output_Width * Output_Height, sizeof(float));
    self.alpha_scale = (float *)calloc(self.presetSize.width * self.presetSize.height * 4, sizeof(float));
    self.rgb_output = (uint8_t *)calloc(self.presetSize.width * self.presetSize.height * 4, sizeof(uint8_t));
    self.rgba_capture = (uint8_t *)calloc(self.presetSize.width * self.presetSize.height * 4, sizeof(uint8_t));
}

- (void)initView {
    UIImageView *imageView = [[UIImageView alloc] initWithFrame:self.view.bounds];
    [self.view addSubview:imageView];
    self.imageView = imageView;
    
    UILabel *label = [[UILabel alloc] initWithFrame:CGRectMake(12, [self naviBarHeight] + 8, 100, 30)];
    label.textColor = [UIColor redColor];
    [self.view addSubview:label];
    self.timeCost = label;
}

- (void)createInstance {
    NSError *error = nil;
    NSString *modelPath = [[NSBundle mainBundle] pathForResource:@"Segmentation.bundle/humanseg-cpu" ofType:@"nb"];
    self.portraitSegmentor = [MMLPortraitSegmentor create:modelPath error:&error];
    if (!self.portraitSegmentor || error) {
        return;
    }
}

#pragma mark - override
- (void)switchImageModelAction:(id)sender {
    [self.navigationController pushViewController:[SegmentationImageViewController new] animated:YES];
}

#pragma mark - delegate

- (void)captureOutput:(AVCaptureOutput*)captureOutput
didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer
       fromConnection:(AVCaptureConnection*)connection {
    if (!self.portraitSegmentor) {
        return;
    }
    [self detectWithSampleBuffer:sampleBuffer];
}

- (void)detectWithSampleBuffer:(CMSampleBufferRef)sampleBuffer {
    NSTimeInterval begin = [[NSDate date] timeIntervalSince1970];
    MMLPSData *output = (MMLPSData *)[self.portraitSegmentor inferWithPixelBuffer:CMSampleBufferGetImageBuffer(sampleBuffer) error:nil];
    NSTimeInterval end = [[NSDate date] timeIntervalSince1970];

    uint8_t *alpahUnit8 = (uint8_t *)output->data;
    if (alpahUnit8 == nullptr) {
        return;
    }
    UIImage *mergeImage = [self mergeCaptureBuffer:sampleBuffer alpha:alpahUnit8];
    dispatch_async(dispatch_get_main_queue(), ^{
        self.imageView.image = mergeImage;
        self.timeCost.text = [NSString stringWithFormat:@"%.2fms", (end - begin) * 1000];
    });
}

//获取samplebuffer的rgba数据 根据模型输出的alpha通道 融合形成新的图片
- (UIImage *)mergeCaptureBuffer:(CMSampleBufferRef)sampleBuffer alpha:(uint8_t *)alphaUint8 {
    //1、sampleBuffer转为image 再获取image对应的rgba数据
    UIImage *captureImage = [self imageFromSampleBuffer:sampleBuffer orientation:UIImageOrientationUp];
    {
        int width = (int)self.presetSize.width;
        int height = (int)self.presetSize.height;
        CGColorSpaceRef colorSpace = CGImageGetColorSpace(captureImage.CGImage);
        CGContextRef contextRef = CGBitmapContextCreate(self.rgba_capture,
                                                        width,
                                                        height,
                                                        8,
                                                        width * 4,
                                                        colorSpace,
                                                        kCGImageAlphaNoneSkipLast | kCGBitmapByteOrderDefault);
        
        CGContextDrawImage(contextRef, CGRectMake(0, 0, width, height), captureImage.CGImage);
        CGContextRelease(contextRef);
    }
    
    //放大模型输出到capture大小
    [self scaleToCaptureSize:alphaUint8];
    
    //利用放大后的alpha及capture_rgba数据进行融合
    return [self mergeCaptureWithWhiteColor];
}

- (UIImage *)imageFromSampleBuffer:(CMSampleBufferRef)sampleBuffer orientation:(UIImageOrientation)orientation{
    CIImage *ciImage = [[CIImage alloc] initWithCVPixelBuffer:CMSampleBufferGetImageBuffer(sampleBuffer)];
    CIContext *context = [[CIContext alloc] init];
    CGImageRef cgimage = [context createCGImage:ciImage fromRect:[ciImage extent]];
    UIImage *image= [UIImage imageWithCGImage:cgimage scale:1.f orientation:orientation];
    CGImageRelease(cgimage);
    return image;
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
    
    for (int i = 0; i < number; i++){
        if (*alpha >= 0.1) {
            output[0] = input[0];
            output[1] = input[1];
            output[2] = input[2];
        } else {
            output[0] = 0;
            output[1] = 255;
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

@end
