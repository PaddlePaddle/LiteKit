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


#import "PaddleGPU+MetalResource.h"
#import <ZipArchive/ZipArchive.h>

static NSString * const PaddleGPUSandBoxStoragePath = @"paddlegpu";             // 解压路径名
// 外层相关
static NSString * const PaddleGPUZipPlistName    = @"pmgpu_zip_config.plist";   // 取值应为pmgpu_zip_config.plist
static NSString * const PaddleGPUVersionOnZip    = @"pmgpu_zip_version";        // 取值应为 xx.xx.xx.xx
static NSString * const PaddleGPUZipName         = @"pmgpu_zip_name";           // 取值应为pmgpu_metal.zip
// 内层相关
static NSString * const PaddleGPUBundlePlistName = @"pmgpu_bundle_config.plist";// 取值应为pmgpu_bundle_config.plist
static NSString * const PaddleGPUVersionOnBundle = @"pmgpu_bundle_version";     // 取值应为 xx.xx.xx.xx
static NSString * const PaddleGPUMetalLibName    = @"pmgpu_metallib_name";      // 取值应为 paddle_mobile.bundle/paddle-mobile-metallib.metallib

// 解析Paddle的Metal路径错误信息封装的domain
NSString * const PaddleGPUParseMetalErrorDomain     = @"PMGPUMetalError";

@implementation PaddleGPU (MetalResource)

#pragma mark - Public

/**
 获取Paddle自定义的MetalLib路径
 @param error 错误信息
 @return paddle自定义的MetalLib路径
 */
+ (NSString *)pm_customMetalLibResourceWithError:(NSError **)error {
    __block NSString *paddleMetalLibPath = nil;
    
#if DEBUG
    NSTimeInterval start = [[NSDate date] timeIntervalSince1970];
#endif
    
    do {
        // 读取外层plist
        NSFileManager *fileManager = [NSFileManager defaultManager];
        NSString *documentPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) firstObject];
        NSString *desFilePath = [documentPath stringByAppendingPathComponent:PaddleGPUSandBoxStoragePath];
        NSString *zipPlistPath = [[NSBundle mainBundle] pathForResource:PaddleGPUZipPlistName ofType:nil];
        if (![fileManager fileExistsAtPath:zipPlistPath]) {
            // 无外层plist,无法判断版本，避免版本不一致导致的crash问题，直接返回
            *error = [NSError errorWithDomain:PaddleGPUParseMetalErrorDomain code:PaddleGPUMetalLibCodeNotExistZipPlist userInfo:nil];
            break;
        }
        NSDictionary *zipPlistMap = [[NSDictionary alloc] initWithContentsOfFile:zipPlistPath];
        if (!zipPlistMap || ![zipPlistMap isKindOfClass:[NSDictionary class]]) {
            // 外层plist文件格式不正确
            *error = [NSError errorWithDomain:PaddleGPUParseMetalErrorDomain code:PaddleGPUMetalLibCodeZipPlistError userInfo:nil];
            break;
        }
        
        // 对解压文件夹进行version提取
        NSString *bundlePlistPath = [desFilePath stringByAppendingPathComponent:PaddleGPUBundlePlistName];
        NSString *versionOnBundle = nil;
        NSDictionary *bundlePlistMap = nil;
        if ([fileManager fileExistsAtPath:bundlePlistPath]) { // 已经解压过
            bundlePlistMap = [[NSDictionary alloc] initWithContentsOfFile:bundlePlistPath];
            if (!bundlePlistMap || ![bundlePlistMap isKindOfClass:[NSDictionary class]]) {
                // 内层plist文件格式不正确
                *error = [NSError errorWithDomain:PaddleGPUParseMetalErrorDomain code:PaddleGPUMetalLibCodeBundlePlistError userInfo:nil];
                break;
            }
            versionOnBundle = [bundlePlistMap objectForKey:PaddleGPUVersionOnBundle];
        }
           
        NSString *versionOnZip = [zipPlistMap objectForKey:PaddleGPUVersionOnZip];
        __block NSError *interanlError = nil;
        if (![versionOnBundle isEqualToString:versionOnZip]) {
            // 版本不同，删除旧包，重新解压新zip包
            NSString *zipName = [zipPlistMap objectForKey:PaddleGPUZipName];
            NSString *zipPath = [[NSBundle mainBundle] pathForResource:zipName ofType:nil];
            if (![fileManager fileExistsAtPath:zipPath]) {
                // 外层zip包不存在
                *error = [NSError errorWithDomain:PaddleGPUParseMetalErrorDomain code:PaddleGPUMetalLibCodeNotExistZip userInfo:nil];
                break;
            }
            
            // sync
            dispatch_sync(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
                if ([fileManager fileExistsAtPath:desFilePath]) {
                    [fileManager removeItemAtPath:desFilePath error:nil];
                }

                BOOL isSucceed = [SSZipArchive unzipFileAtPath:zipPath toDestination:desFilePath];
                if (isSucceed) {
                    // parse
                    NSString *newBundlePlistPath = [desFilePath stringByAppendingPathComponent:PaddleGPUBundlePlistName];
                    NSDictionary *newBundlePlistMap = [[NSDictionary alloc] initWithContentsOfFile:newBundlePlistPath];
                    paddleMetalLibPath = [PaddleGPU pm_parseMetalLibPath:newBundlePlistMap metalBundleRootPath:desFilePath error:&interanlError];
                } else {
                    // 解压失败
                    interanlError = [NSError errorWithDomain:PaddleGPUParseMetalErrorDomain code:PaddleGPUMetalLibCodeUnzipError userInfo:nil];
                }
            });
            
        } else {
            // 版本相同，使用原有解压包路径下的metallib
            paddleMetalLibPath = [PaddleGPU pm_parseMetalLibPath:bundlePlistMap metalBundleRootPath:desFilePath error:&interanlError];
        }
        if (error != NULL && interanlError != nil) {
            *error = interanlError;
        }
        
    } while (0);
    
#if DEBUG
    NSTimeInterval end = [[NSDate date] timeIntervalSince1970];
    NSLog(@"search for paddle gpu metallib cost:%.3f", (end-start) * 1000);
#endif
    
    return paddleMetalLibPath;
}

#pragma mark - Private

/**
 根据bundlePlistMap解析metalLib路径
 @param bundlePlistMap Paddle对应的Bundle Plist
 @param metalBundlePath Paddle对应的Bundle根路径
 @param error 错误信息
 @return bundlePlistMap解析metalLib路径
*/
+ (NSString *)pm_parseMetalLibPath:(NSDictionary *)bundlePlistMap
               metalBundleRootPath:(NSString *)metalBundlePath
                             error:(NSError **)error {
    if (!bundlePlistMap || ![bundlePlistMap isKindOfClass:[NSDictionary class]]) {
        return nil;
    }
    NSString *metalLibName = [bundlePlistMap objectForKey:PaddleGPUMetalLibName];
    NSString *metalLibPath = [metalBundlePath stringByAppendingPathComponent:metalLibName];
    NSFileManager *fileManager = [NSFileManager defaultManager];
    if (![fileManager fileExistsAtPath:metalLibPath]) {
        metalLibPath = nil;
        *error = [NSError errorWithDomain:PaddleGPUParseMetalErrorDomain code:PaddleGPUMetalLibCodeNotExistBundle userInfo:nil];
    }
    return metalLibPath;
}


@end
