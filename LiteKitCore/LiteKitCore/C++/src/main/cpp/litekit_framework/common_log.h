/**
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

#ifndef LITEKIT_FRAMEWORK_ANDROID_COMMON_H
#define LITEKIT_FRAMEWORK_ANDROID_COMMON_H

#include "platform_switch.h"
#include <stdio.h>
#include <string.h>

// 打Release包请关闭DEBUG
#ifndef LITEKIT_OPEN_LOG
    #define LOGI(format, ...)
    #define LOGW(format, ...)
    #define LOGE(format, ...)
#else //else LITEKIT_DEBUG
    #ifdef PLATFORM_ANDROID
        #include <android/log.h>
        #define LOGI(format, ...) __android_log_print(ANDROID_LOG_INFO, "litekit_framework", \
                format, ##__VA_ARGS__)
        #define LOGW(format, ...) __android_log_print(ANDROID_LOG_WARN, "litekit_framework", \
                format, ##__VA_ARGS__)
        #define LOGE(format, ...) __android_log_print(ANDROID_LOG_ERROR, "litekit_framework", \
                "Error: " format, ##__VA_ARGS__)
    #else //else  PLATFORM_ANDROID
        #define LOGI(format, ...) printf("【litekit】 Info: " format, ##__VA_ARGS__)
        #define LOGW(format, ...) printf("【litekit】 Warn: " format, ##__VA_ARGS__)
        #define LOGE(format, ...) printf("【litekit】 Error: " format, ##__VA_ARGS__)
    #endif
#endif //endif LITEKIT_DEBUG


/// LiteKit log
#ifdef LITEKIT_OPEN_LOG

#define FUNC_BEGIN_WITHTIME {litekit_framework::log::litekit_log(__PRETTY_FUNCTION__, "Begin");}
#define FUNC_END_WITHTIME {litekit_framework::log::litekit_log(__PRETTY_FUNCTION__, "End");}

#define CALL_BEGIN_WITHTIME(methodName) {litekit_framework::log::litekit_log(methodName, "Begin");}
#define CALL_END_WITHTIME(methodName) {litekit_framework::log::litekit_log(methodName, "End");}

namespace litekit_framework {
namespace log {

void litekit_log(const char *perty_funcname, const char *stage);

}
}

#else

#define FUNC_BEGIN_WITHTIME
#define FUNC_END_WITHTIME

#define CALL_BEGIN_WITHTIME(methodName)
#define CALL_END_WITHTIME(methodName)

#endif


#endif  // LITEKIT_FRAMEWORK_ANDROID_COMMON_H
