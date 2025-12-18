#ifndef _RIME_STATUS_CPP_
#define _RIME_STATUS_CPP_

#include "rime_jni.hpp"

class JRimeStatus : public JavaClass<JRimeStatus> {
    using namespace facebook::jni;
private:
    rime_status_t status;
public:
    static constexpr auto kJavaDescriptor = "Licu.astronot233.rime.RimeStatus;";
    local_ref<JString> getSchemaId() {
        return make_jstring(status.schema_id);
    }
    local_ref<JString> getSchemaName() {
        return make_jstring(status.schema_name);
    }
    jboolean isDisabled() {
        return status.is_disabled;
    }
    jboolean isComposing() {
        return status.is_composing;
    }
    jboolean isAsciiMode() {
        return status.is_ascii_mode;
    }
    jboolean isFullShape() {
        return status.is_full_shape;
    }
    jboolean isSimplified() {
        return status.is_simplified;
    }
    jboolean isTraditional() {
        return status.is_traditional;
    }
    jboolean isAsciiPunct() {
        return status.is_ascii_punct;
    }
};

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    return jni::initialize(vm, [] {
        JRimeStatus::registerNatives();
    });
}

#endif