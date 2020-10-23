//
//  BaseViewController.m
//  MMLKitDemo
//
//  Created by Baidu on 2020/7/2.
//  Copyright © 2020 Baidu Co.,Ltd. . All rights reserved.
//

#import "BaseViewController.h"
#import <CoreMotion/CoreMotion.h>

@interface BaseViewController ()
@property (nonatomic, assign) BOOL isFrontCamera;
@property (nonatomic, strong) AVCaptureSession *session;
@property (nonatomic, strong) AVCaptureVideoPreviewLayer *videoPreviewLayer;
@property (nonatomic, strong) AVCaptureVideoDataOutput *videoDataOutput;
@property (nonatomic, strong) dispatch_queue_t sampleBufferCallbackQueue;
@property (nonatomic, strong) CMMotionManager *motionManager;   //设备传感器

/**
 * 是否需要相机
 */
- (BOOL)isNeedCamera;

/**
 * 是否需要相机
 */
- (AVCaptureSessionPreset)captureSessionPreset;

@end

@implementation BaseViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor whiteColor];
    self.isFrontCamera = YES;
    [self initViews];
    [self initMotion];
    [self initCamera:^{
        [self update];
    }];
}

-(void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    [self startCamera];
}

-(void)viewWillDisappear:(BOOL)animated {
    [super viewDidDisappear:YES];
    [self stopCamera];
}

-(void)dealloc {
    [[UIDevice currentDevice] endGeneratingDeviceOrientationNotifications];
}

#pragma mark - init

- (void)initViews {
    //切换摄像头
    UIBarButtonItem *useCamera = [[UIBarButtonItem alloc] initWithTitle:@"相机"
                                                                  style:UIBarButtonItemStylePlain
                                                                 target:self
                                                                 action:@selector(switchCameraModeAction:)];
    //切换到静态图VC
    UIBarButtonItem *useImage = [[UIBarButtonItem alloc] initWithTitle:@"图片"
                                                                 style:UIBarButtonItemStylePlain
                                                                target:self
                                                                action:@selector(switchImageModelAction:)];
    self.navigationItem.rightBarButtonItems = @[useCamera, useImage];
    
    //由于是调用view的drawrect绘制，所以detectview的frame需要与layer的frame一致
    self.detectView = [self inferenceContentView];
    self.detectView.presetSize = [self captureSessionPresetSize];
    self.detectView.backgroundColor = [UIColor clearColor];
    [self.view addSubview:self.detectView];
}

- (void)initCamera:(void (^)(void))complete {
    if (![self isNeedCamera]) {
        return;
    }
    AVAuthorizationStatus authStatus = [AVCaptureDevice authorizationStatusForMediaType:AVMediaTypeVideo];
    if (authStatus ==AVAuthorizationStatusRestricted || authStatus ==AVAuthorizationStatusDenied) {
        NSLog(@"相机权限申请拒绝");
        return;
    } else if (authStatus == AVAuthorizationStatusNotDetermined) {
        [AVCaptureDevice requestAccessForMediaType:AVMediaTypeVideo completionHandler:^(BOOL granted) {
            if (!granted) {
                NSLog(@"相机权限申请拒绝");
                return;
            }
            dispatch_async(dispatch_get_main_queue(), ^{
                [self setupAVCapture];
                complete();
            });
        }];
    } else {
        [self setupAVCapture];
        complete();
    }
}

- (void)initMotion {
    if (![self isNeedCamera]) {
        return;
    }
    
    // 添加设备方向变化监听
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(deviceOrientationDidChange:)
                                                 name:UIDeviceOrientationDidChangeNotification
                                               object:nil];
    [[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
    
    // 获取设备陀螺仪数据
    self.motionManager = [[CMMotionManager alloc] init];
    if ([self.motionManager isDeviceMotionAvailable]) {
        [self.motionManager startDeviceMotionUpdates];
    }
}

#pragma mark - action
- (void)switchCameraModeAction:(id)sender  {
    [self stopCamera];
    NSArray *inputs = self.session.inputs;
    for (AVCaptureDeviceInput *input in inputs ) {
        AVCaptureDevice *device = input.device;
        if ( [device hasMediaType:AVMediaTypeVideo] ) {
            AVCaptureDevice *newCamera = [self cameraWithPosition:!self.isFrontCamera];
            self.isFrontCamera = !self.isFrontCamera;
            AVCaptureDeviceInput *newInput = [AVCaptureDeviceInput deviceInputWithDevice:newCamera error:nil];
            [self.session beginConfiguration];
            [self.session removeInput:input];
            [self.session addInput:newInput];
            [self.session commitConfiguration];
            break;
        }
    }
    [self updateVideoDataOutput];
    [self startCamera];
}

- (void)switchImageModelAction:(id)sender {

}

#pragma mark - override
- (BOOL)isNeedCamera {
    return YES;
}

-(AVCaptureSessionPreset)captureSessionPreset {
    return AVCaptureSessionPreset1280x720;
}

- (BaseDetectView*)inferenceContentView {
    return nil;
}

- (void)imageModeAction:(id)sender {
    
}

#pragma mark - camera

- (void)setupAVCapture {
    // 创建并配置session
    self.session = [[AVCaptureSession alloc] init];
    [self.session setSessionPreset:[self captureSessionPreset]];
    // 获取并配置摄像头，默认前置
    NSError *error = nil;
    AVCaptureDevice *device = [AVCaptureDevice defaultDeviceWithDeviceType:AVCaptureDeviceTypeBuiltInWideAngleCamera
                                                                 mediaType:AVMediaTypeVideo
                                                                  position:AVCaptureDevicePositionFront];
    // 创建、配置并添加输入源
    AVCaptureDeviceInput *deviceInput = [AVCaptureDeviceInput deviceInputWithDevice:device error:&error];
    if (error) {
        NSLog(@"Failed to initialize AVCaptureDeviceInput. Note: This app doesn't work with simulator");
        assert(NO);
        return;
    }
    if ([self.session canAddInput:deviceInput]) {
        [self.session addInput:deviceInput];
    }
    // 创建、配置并添加输出源
    self.videoDataOutput = [[AVCaptureVideoDataOutput alloc] init];
    NSDictionary* ouputSettting = [NSDictionary dictionaryWithObject:[NSNumber numberWithInt:kCMPixelFormat_32BGRA]
                                                              forKey:(id)kCVPixelBufferPixelFormatTypeKey];
    [self.videoDataOutput setVideoSettings:ouputSettting];
    if ([self.session canAddOutput:self.videoDataOutput]) {
        [self.session addOutput:self.videoDataOutput];
    }
    [self.videoDataOutput setAlwaysDiscardsLateVideoFrames:YES];
    //处理队列
    self.sampleBufferCallbackQueue = dispatch_queue_create("com.baidu.MMLKit.camera", DISPATCH_QUEUE_SERIAL);
    [self.videoDataOutput setSampleBufferDelegate:self queue:self.sampleBufferCallbackQueue];
    
    // 创建、配置并添加预览layer
    self.videoPreviewLayer = [[AVCaptureVideoPreviewLayer alloc] initWithSession:self.session];
    [self.videoPreviewLayer setBackgroundColor:[[UIColor blackColor] CGColor]];
    [self.videoPreviewLayer setVideoGravity:AVLayerVideoGravityResize];
    [self.videoPreviewLayer setFrame:CGRectMake(0, 0, self.view.bounds.size.width, self.view.bounds.size.height)];
    [self.view.layer addSublayer:self.videoPreviewLayer];
    
    // 开启session进行视频捕获
//    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(3 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
//        [self startCamera];
//    });
    [self startCamera];
}

- (void)startCamera {
    if (self.session) {
        [self.session startRunning];
    }
}

- (void)stopCamera {
    if (self.session) {
        [self.session stopRunning];
    }
}

#pragma mark - notification
- (BOOL)deviceOrientationDidChange:(NSNotification*)notification {
    //横竖屏切换是更新以下内容：layer的frame、layer上的相机流、camera输出数据（即delegate回调的buffer内容更新方向）
    [self update];
    return YES;
}

#pragma mark - UI
- (BOOL)update {
    [self updateViewLayout];
    [self updatePreviewLayer];
    [self updateVideoDataOutput];
    return YES;
}

//横竖屏切换时：显示相机流的layer更新frame
- (void)updateViewLayout {
    BOOL isCamera = [self isNeedCamera];
    CGSize presetSize = [self captureSessionPresetSize];
    CGFloat screenWidth = [[UIScreen mainScreen] bounds].size.width;
    CGFloat screenHeight = [[UIScreen mainScreen] bounds].size.height;
    if (screenWidth < screenHeight) {
        CGFloat adjustHeight = presetSize.height * screenWidth / presetSize.width;  // 适应宽
        if (isCamera) {
            self.videoPreviewLayer.frame = CGRectMake(0, 0, screenWidth, adjustHeight);
        }
        self.detectView.frame = CGRectMake(0, 0, screenWidth, adjustHeight);
    } else {
        CGFloat adjustWidth = screenHeight * presetSize.height / presetSize.width;  // 适应宽
        if (isCamera) {
            self.videoPreviewLayer.frame = CGRectMake(0, 0, adjustWidth, screenHeight);
        }
        self.detectView.frame = CGRectMake(0, 0, adjustWidth, screenHeight);
    }
    [self.view bringSubviewToFront:self.detectView];
}

//横竖屏切换时：显示相机流的layer更新输出内容的方向
- (void)updatePreviewLayer {
    if (![self isNeedCamera]) {
        return;
    }
    AVCaptureConnection *connectionLayer = self.videoPreviewLayer.connection;
    [self updateVideoOrientation:connectionLayer];
}

//横竖屏切换时：相机流session输出数据更新方向
- (void)updateVideoDataOutput {
    if (![self isNeedCamera] || self.videoDataOutput.connections.count == 0) {
        return;
    }
    AVCaptureConnection *connection = self.videoDataOutput.connections.firstObject;
    [self updateVideoOrientation:connection];
}

#pragma mark - internal
//导航栏高度+状态栏高度
- (float)naviBarHeight {
    return self.navigationController.navigationBar.frame.size.height + [[UIApplication sharedApplication] statusBarFrame].size.height;
}

- (CGSize)captureSessionPresetSize {
    CGSize size = CGSizeZero;
    AVCaptureSessionPreset preset = [self captureSessionPreset];
    if ([preset isEqualToString:AVCaptureSessionPreset1920x1080]) {
        size = CGSizeMake(1080, 1920);
    } else if ([preset isEqualToString:AVCaptureSessionPreset1280x720]) {
        size = CGSizeMake(720, 1280);
    } else if ([preset isEqualToString:AVCaptureSessionPreset640x480]) {
        size = CGSizeMake(480, 640);
    }
    return size;
}

- (void)updateVideoOrientation:(AVCaptureConnection *)connection {
    UIDeviceOrientation orientation = [UIDevice currentDevice].orientation;
    switch (orientation) {
        case UIDeviceOrientationPortraitUpsideDown:
            NSLog(@"上下顛倒");
//            if (connection.videoOrientation != AVCaptureVideoOrientationPortraitUpsideDown) {
//                connection.videoOrientation = AVCaptureVideoOrientationPortraitUpsideDown;
//            }
            break;
        case UIDeviceOrientationLandscapeLeft:
            NSLog(@"向左橫置");
            if (connection.videoOrientation != AVCaptureVideoOrientationLandscapeRight) {
                connection.videoOrientation = AVCaptureVideoOrientationLandscapeRight;
            }
            break;
        case UIDeviceOrientationLandscapeRight:
            NSLog(@"向右橫置");
            if (connection.videoOrientation != AVCaptureVideoOrientationLandscapeLeft) {
                connection.videoOrientation = AVCaptureVideoOrientationLandscapeLeft;
            }
            break;
        case UIDeviceOrientationPortrait:
            NSLog(@"直立");
            if (connection.videoOrientation != AVCaptureVideoOrientationPortrait) {
                connection.videoOrientation = AVCaptureVideoOrientationPortrait;
            }
            break;
        default:
            break;
    }
    if (connection.supportsVideoMirroring && !connection.automaticallyAdjustsVideoMirroring) {
        connection.videoMirrored = self.isFrontCamera;
    }
}

- (AVCaptureDevice *)cameraWithPosition:(BOOL)isFrontCamera {
    AVCaptureDevicePosition devicePosition = isFrontCamera?AVCaptureDevicePositionFront:AVCaptureDevicePositionBack;
    NSArray *devices = [AVCaptureDevice devicesWithMediaType:AVMediaTypeVideo];
    for (AVCaptureDevice *device in devices ) {
        if (device.position == devicePosition) {
            return device;
        }
    }
    return nil;
}

@end
