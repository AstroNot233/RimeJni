#include "rime_jni.hpp"

extern "C" {

    void registerAllNatives() {
        rime::jni::JRimeCore::RegisterNatives();
    }

    JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
        return facebook::jni::initialize(vm, registerAllNatives);
    }

}
