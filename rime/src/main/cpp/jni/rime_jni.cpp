#include "rime_jni.hpp"
#include "fbjni_alias.hpp"

namespace rime {
class JRime : public fbjni::HybridClass<JRime> {
private:
    friend HybridBase;
    std::unique_ptr<RimeApi> const rime { rime_get_api() };
    global_ref<jobject> const callback;
    RimeSessionId session {};
    std::string const sharedDataDir;
    std::string const userDataDir;
    std::string const appName;
    RimeTraits const traits;

public:
    static constexpr auto kJavaDescriptor { "Licu/astronot233/rime/Rime;" };
    static void RegisterNatives() {
        registerHybrid({
            makeNativeMethod("create",              JRime::create               ),
            makeNativeMethod("startup",             JRime::startup              ),
            makeNativeMethod("deploySchema",        JRime::deploySchema         ),
            makeNativeMethod("deployConfigFile",    JRime::deployConfigFile     ),
            makeNativeMethod("processKey",          JRime::processKey           ),
            makeNativeMethod("simulateKeySequence", JRime::simulateKeySequence  ),
            makeNativeMethod("commitComposition",   JRime::commitComposition    ),
            makeNativeMethod("clearComposition",    JRime::clearComposition     ),
//            makeNativeMethod("commitProto",         JRime::commitProto          ),
        });
    }

    static local_ref<JRime::javaobject> create
        (alias_ref<JClass> /*self*/, jstring sharedDataDir, jstring userDataDir, jstring appName, jobject callback) {
        return newObjectCxxArgs(sharedDataDir, userDataDir, appName, callback);
    }

    jboolean startup(jboolean const fullCheck) {
        rime->setup(const_cast<RimeTraits *>(&traits));
        rime->initialize(const_cast<RimeTraits *>(&traits));
        rime->set_notification_handler(notificationHandler, const_cast<JRime *>(this));
        return rime->start_maintenance(fullCheck);
    }

    jboolean deploySchema(jstring schemaFile) {
        auto const env { fbjni::Environment::current() };
        auto const sf { env->GetStringUTFChars(schemaFile, nullptr) };
        auto const result { static_cast<jboolean>(rime->deploy_schema(sf)) };
        env->ReleaseStringUTFChars(schemaFile, sf);
        return result;
    }
    jboolean deployConfigFile(jstring fileName, jstring versionKey) {
        auto const env { fbjni::Environment::current() };
        auto const fn { env->GetStringUTFChars(fileName, nullptr) };
        auto const vk { env->GetStringUTFChars(versionKey, nullptr) };
        auto const result { static_cast<jboolean>(rime->deploy_config_file(fn, vk)) };
        env->ReleaseStringUTFChars(fileName, fn);
        env->ReleaseStringUTFChars(versionKey, vk);
        return result;
    }

    jboolean processKey(jint keyCode, jint mask) {
        return rime->process_key(getSession(), keyCode, mask);
    }
    jboolean simulateKeySequence(jstring sequence) {
        auto const env { fbjni::Environment::current() };
        auto const s { env->GetStringUTFChars(sequence, nullptr) };
        auto const result { static_cast<jboolean>(rime->simulate_key_sequence(getSession(), s)) };
        env->ReleaseStringUTFChars(sequence, s);
        return result;
    }

    jboolean commitComposition() {
        return rime->commit_composition(getSession());
    }
    void clearComposition() {
        return rime->clear_composition(getSession());
    }

    void commitProto(RIME_PROTO_BUILDER * commitBuilder) {
        return rime->commit_proto(getSession(), commitBuilder);
    }

private:
    JRime(alias_ref<jstring> sharedDataDir, alias_ref<jstring> userDataDir,
          alias_ref<jstring> appName,       alias_ref<jobject> callback) :
          callback { make_global(callback) },       sharedDataDir { sharedDataDir->toStdString() },
          userDataDir { userDataDir->toStdString() },   appName { appName->toStdString() },
          traits {
            sizeof(RimeTraits) - sizeof(RimeTraits::data_size),
            this->sharedDataDir.c_str(),
            this->userDataDir.c_str(),
            "Rime for Android",
            "astronot233",
            rime ? rime->get_version() : "Unknown",
            this->appName.c_str(),
            nullptr,
            0,
            "",
            nullptr,
            nullptr
          } {}
    RimeSessionId getSession(bool const newSession = false) {
        if (newSession && session)
            rime->destroy_session(session);
        if (!session)
            session = rime->create_session();
        return session;
    }
    static void notificationHandler(void * const context_object, RimeSessionId const session_id,
        char const * const message_type, char const * const message_value) {
        auto callback = static_cast<JRime *>(context_object)->callback;
        return (callback->getClass()->getMethod<void(jlong, jstring, jstring)>("handleMessage"))
                (callback.get(), session_id ,fbjni::make_jstring(message_type).get(), fbjni::make_jstring(message_value).get());
    }
};
}

extern "C" {
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    using namespace rime;
    return fbjni::initialize(vm, []() {
        JRime::RegisterNatives();
    });
}
}
