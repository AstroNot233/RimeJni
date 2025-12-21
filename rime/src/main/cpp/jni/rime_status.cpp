#ifndef _RIME_STATUS_CPP_
#define _RIME_STATUS_CPP_

#include "rime_jni.hpp"
#include "fbjni_alias.hpp"

namespace rime {
class JRimeStatus : public fbjni::HybridClass<JRimeStatus> {
private:
    friend HybridBase;
    RimeStatus status;
public:
    static constexpr auto kJavaDescriptor = "Licu/astronot233/rime/RimeStatus;";

    local_ref<jstring> getSchemaId() { return fbjni::make_jstring(status.schema_id); }
    local_ref<jstring> getSchemaName() { return fbjni::make_jstring(status.schema_name); }
    jboolean isDisabled() { return status.is_disabled; }
    jboolean isComposing() { return status.is_composing; }
    jboolean isAsciiMode() { return status.is_ascii_mode; }
    jboolean isFullShape() { return status.is_full_shape; }
    jboolean isSimplified() { return status.is_simplified; }
    jboolean isTraditional() { return status.is_traditional; }
    jboolean isAsciiPunct() { return status.is_ascii_punct; }
};

//void RimeStatus_RegisterNatives() { return JRimeStatus::RegisterNatives(); }

}
#endif