//
// Created by cuihao02@baidu.com on 2020/8/24.
//

#ifndef MMLCORE_APP_SRC_MAIN_CPP_PREPROCESS_H_
#define MMLCORE_APP_SRC_MAIN_CPP_PREPROCESS_H_

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

namespace imba {

extern "C"
JNIEXPORT jfloatArray JNICALL
Java_com_baidu_mml_demo_MainActivity_preprocess(JNIEnv *env,
                                               jobject thiz,
                                               jbyteArray data,
                                               jint imgWidth,
                                               int imgHeight);

}

#ifdef __cplusplus
}
#endif

#endif //MMLCORE_APP_SRC_MAIN_CPP_PREPROCESS_H_
