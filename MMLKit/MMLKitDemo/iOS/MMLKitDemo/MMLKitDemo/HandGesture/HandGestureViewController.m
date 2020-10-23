//
//  HandGestureViewController.m
//  MMLKitDemo
//
//  Created by Baidu on 2020/7/2.
//  Copyright © 2020 Baidu Co.,Ltd. . All rights reserved.
//

#import "HandGestureViewController.h"
#import "HandGestureRecognizeView.h"
#import "HandGestureImageViewController.h"

@interface HandGestureViewController ()
@property (strong, nonatomic) UILabel *timeCost;
@property (nonatomic, strong) MMLHandGestureDetector *gestureRecognizer;
@end

@implementation HandGestureViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    [self initView];
    [self createInstance];
}

- (void)initView {
    UILabel *label = [[UILabel alloc] initWithFrame:CGRectMake(12, [self naviBarHeight] + 8, 100, 30)];
    label.textColor = [UIColor redColor];
    [self.view addSubview:label];
    self.timeCost = label;
}

- (void)createInstance {
    NSError *error;
    NSString *modelPath = [[[NSBundle mainBundle] bundlePath] stringByAppendingString:@"/HandGesture.bundle/"];
    self.gestureRecognizer = [MMLHandGestureDetector createGestureDetectorWithModelPath:modelPath error:&error];
    if (!self.gestureRecognizer || error) {
        return;
    }
}

#pragma mark - override
- (BaseDetectView*)inferenceContentView {
    HandGestureRecognizeView *detectView = [[HandGestureRecognizeView alloc] init];
    return detectView;
}

- (void)switchImageModelAction:(id)sender {
    [self.navigationController pushViewController:[HandGestureImageViewController new] animated:YES];
}

#pragma mark - delegate

- (void)captureOutput:(AVCaptureOutput*)captureOutput
didOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer
       fromConnection:(AVCaptureConnection*)connection {
    [self detectWithSampleBuffer:sampleBuffer];
}

- (void)detectWithSampleBuffer:(CMSampleBufferRef)sampleBuffer {
    NSTimeInterval begin = [[NSDate date] timeIntervalSince1970];
    [self.gestureRecognizer detectWithSampleBuffer:sampleBuffer
                                          complete:^(MMLHandGestureDetectResult *result, NSError *error) {
        NSTimeInterval end = [[NSDate date] timeIntervalSince1970];
        dispatch_async(dispatch_get_main_queue(), ^{
            [self outputDetectResult:result];
            [(HandGestureRecognizeView*)self.detectView updateResultData:result];
            self.timeCost.text = [NSString stringWithFormat:@"%.2fms", (end - begin) * 1000];
        });
    }];
}

- (void)outputDetectResult:(MMLHandGestureDetectResult *)result {
    if (result) {
        NSLog(@"Gesture Detect Result:\n"
              "\t\t\t----Detect Label Index:%d\n"
              "\t\t\t----Hand Box Rect: (xStart:%f yStart:%f width:%f height:%f)\n"
              "\t\t\t----Finger Point Position: (x:%f y:%f)",
              (int)result.label,
              result.handBoxRect.origin.x,
              result.handBoxRect.origin.y,
              result.handBoxRect.size.width,
              result.handBoxRect.size.height,
              result.fingerPoint.x,
              result.fingerPoint.y);
    }
}

@end
