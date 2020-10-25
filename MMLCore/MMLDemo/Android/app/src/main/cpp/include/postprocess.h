//
// Created by cuihao02@baidu.com on 2020/8/24.
//

#ifndef MMLCORE_APP_SRC_MAIN_CPP_INCLUDE_POSTPROCESS_H_
#define MMLCORE_APP_SRC_MAIN_CPP_INCLUDE_POSTPROCESS_H_

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

namespace imba {

extern "C"
JNIEXPORT jfloatArray JNICALL
Java_com_baidu_mml_demo_MainActivity_postprocess(JNIEnv *env,
                                                 jobject thiz,
                                                 jfloatArray data0,
                                                 jfloatArray data1,
                                                 jfloatArray data2,
                                                 jfloatArray data3,
                                                 jfloatArray data4,
                                                 jint imgWidth,
                                                 jint imgHeight
);

}

#ifdef __cplusplus
}
#endif

#endif //MMLCORE_APP_SRC_MAIN_CPP_INCLUDE_POSTPROCESS_H_
