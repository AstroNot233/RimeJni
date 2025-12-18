#include "rime_jni.hpp"

using namespace facebook::jni;

class JRime : public JavaClass<JRime> {
public:
    static constexpr auto kJavaDescriptor = "Licu.astronot233.rime.Rime;";

    // 使用 fbjni 的类型安全方法
    local_ref<JString> getSchema() {
        // 这里可以调用 librime 的 C++ API
        RimeSchemaInfo info;
        if (RimeGetSchemaInfo(&info)) {
            return make_jstring(info.schema_id);
        }
        return nullptr;
    }
};

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    return jni::initialize(vm, [] {
        JRimeStatus::registerNatives();
    });
}