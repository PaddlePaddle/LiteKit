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

#ifndef MML_FRAMEWORK_ANDROID_COMMON_H
#define MML_FRAMEWORK_ANDROID_COMMON_H

#include "platform_switch.h"
#include <stdio.h>
#include <string.h>

// 打Release包请关闭DEBUG
#ifndef MML_OPEN_LOG
    #define LOGI(format, ...)
    #define LOGW(format, ...)
    #define LOGE(format, ...)
#else //else MML_DEBUG
    #ifdef PLATFORM_ANDROID
        #include <android/log.h>
        #define LOGI(format, ...) __android_log_print(ANDROID_LOG_INFO, "mml_framework", \
                format, ##__VA_ARGS__)
        #define LOGW(format, ...) __android_log_print(ANDROID_LOG_WARN, "mml_framework", \
                format, ##__VA_ARGS__)
        #define LOGE(format, ...) __android_log_print(ANDROID_LOG_ERROR, "mml_framework", \
                "Error: " format, ##__VA_ARGS__)
    #else //else  PLATFORM_ANDROID
        #define LOGI(format, ...) printf("【mml】 Info: " format, ##__VA_ARGS__)
        #define LOGW(format, ...) printf("【mml】 Warn: " format, ##__VA_ARGS__)
        #define LOGE(format, ...) printf("【mml】 Error: " format, ##__VA_ARGS__)
    #endif
#endif //endif MML_DEBUG


/// MML log
#ifdef MML_OPEN_LOG

#define FUNC_BEGIN_WITHTIME {mml_framework::log::mml_log(__PRETTY_FUNCTION__, "Begin");}
#define FUNC_END_WITHTIME {mml_framework::log::mml_log(__PRETTY_FUNCTION__, "End");}

#define CALL_BEGIN_WITHTIME(methodName) {mml_framework::log::mml_log(methodName, "Begin");}
#define CALL_END_WITHTIME(methodName) {mml_framework::log::mml_log(methodName, "End");}

namespace mml_framework {
namespace log {

void mml_log(const char *perty_funcname, const char *stage);

}
}

#else

#define FUNC_BEGIN_WITHTIME
#define FUNC_END_WITHTIME

#define CALL_BEGIN_WITHTIME(methodName)
#define CALL_END_WITHTIME(methodName)

#endif


#endif  // MML_FRAMEWORK_ANDROID_COMMON_H
