//
// Created by cuihao02@baidu.com on 2020/8/22.
//

#ifndef MMLCORE_MML_FEED_INPUT_DATA_H
#define MMLCORE_MML_FEED_INPUT_DATA_H
#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

namespace mmlcore {

JNIEXPORT void JNICALL
feedFloatInputData(JNIEnv *env,
              jobject type,
              jlong nativeMachineHandler,
              jfloatArray data,
              jint inputBatch,
              jint inputChannel,
              jint inputHeight,
              jint inputWidth,
              jint inputGraphId);

JNIEXPORT void JNICALL
feedByteInputData(JNIEnv *env,
              jobject type,
              jlong nativeMachineHandler,
              jbyteArray data,
              jint inputBatch,
              jint inputChannel,
              jint inputHeight,
              jint inputWidth,
              jint inputGraphId);

JNIEXPORT void JNICALL
feedIntInputData(JNIEnv *env,
              jobject type,
              jlong nativeMachineHandler,
              jintArray data,
              jint inputBatch,
              jint inputChannel,
              jint inputHeight,
              jint inputWidth,
              jint inputGraphId);

JNIEXPORT void JNICALL
feedLongInputData(JNIEnv *env,
              jobject type,
              jlong nativeMachineHandler,
              jlongArray data,
              jint inputBatch,
              jint inputChannel,
              jint inputHeight,
              jint inputWidth,
              jint inputGraphId);

}

#ifdef __cplusplus
}
#endif

#endif //MMLCORE_MML_FEED_INPUT_DATA_H
