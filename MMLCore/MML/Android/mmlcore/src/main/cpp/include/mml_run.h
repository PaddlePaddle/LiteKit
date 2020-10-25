//
// Created by cuihao02@baidu.com on 2020/8/22.
//

#ifndef MMLCORE_MML_RUN_H
#define MMLCORE_MML_RUN_H

#ifdef __cplusplus
extern "C" {
#endif

namespace mmlcore {

extern "C"
JNIEXPORT void JNICALL
Java_com_baidu_mmlcore_MMLBaseMachine_run(JNIEnv *env, jobject type, jlong nativeMachineHandler);

}

#ifdef __cplusplus
}
#endif

#endif //MMLCORE_MML_RUN_H
