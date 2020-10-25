//
//  common_log.cpp
//  mml_framework_ios
//
//  Created by Baidu Co.Ltd. on 2020/3/11.
//  Copyright © 2020 Baidu Co.Ltd. All rights reserved.
//

#include <stdio.h>
#include "common_log.h"
#include <sys/time.h>

namespace mml_framework {
namespace log {

void mml_log(const char *perty_funcname, const char *stage) {
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    LOGI("TS: %ld.%d  |  stage: %s  |  funcname: %s \n", tv.tv_sec, tv.tv_usec, stage, perty_funcname);
}

}
}
