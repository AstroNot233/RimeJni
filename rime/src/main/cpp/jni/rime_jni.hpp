#ifndef RIME_JNI_HPP
#define RIME_JNI_HPP

#include <memory>

#include <fbjni/fbjni.h>
#include <librime/rime_api.h>
#include <librime/rime/key_table.h>

namespace rime {

    constexpr auto MAX_BUFFER_LENGTH { 1ULL << 10 };

    struct RimeTraitsAndroid : public RimeTraits {
        RimeTraitsAndroid(char const * sharedDataDir, char const * userDataDir, char const * distributionVersion, char const * appName);
        ~RimeTraitsAndroid();
    };

    class JRimeCore : public facebook::jni::HybridClass<JRimeCore> {
    private:
        friend HybridBase;
        std::unique_ptr<RimeApi> const rime { rime_get_api() };
        facebook::jni::global_ref<facebook::jni::JObject> const notification;
        std::shared_ptr<RimeTraitsAndroid> const traits;
        RimeSessionId session {};

    public:
        static constexpr auto kJavaDescriptor { "Licu/astronot233/rime/RimeCore;" };

// Factory method
        static facebook::jni::local_ref<JRimeCore::javaobject> create (facebook::jni::alias_ref<facebook::jni::JClass> /*self*/,
                jstring sharedDataDir, jstring userDataDir, jstring appName, jobject callback);

// Lifecycle
        jboolean startup(jboolean fullCheck);
        void shutdown();
        jboolean syncUserData();

// IO behavior
        jboolean processKey(jint keyCode, jint mask);
        jboolean simulateKeySequence(jstring sequence);
        jboolean commitComposition();
        void clearComposition();

// Option
        void setOption(jstring option, jboolean value);
        jboolean getOption(jstring option);
        void setProperty(jstring property, jstring value);
        jstring getProperty(jstring property);

// Schema
        jboolean deploySchema(jstring schemaFile);
        jobject getSchemaList();
        jstring getCurrentSchemaId();
        jboolean selectSchema(jstring schemaId);

// Candidate and page
        jboolean selectCandidate(jint index);
        jboolean deleteCandidate(jint index);
        jboolean highlightCandidate(jint index);
        jboolean changePage(jboolean backward);

// Config
        jboolean deployConfigFile(jstring fileName, jstring versionKey);

        static void RegisterNatives();

    private:
        JRimeCore(jstring sharedDataDir, jstring userDataDir, jstring appName, jobject callback);
        RimeSessionId getSession(bool newSession = false);
        static void notificationHandler(void * context_object, RimeSessionId session_id, char const * message_type, char const * message_value);
    };

    struct JRimeSchemaInfo : public facebook::jni::JavaClass<JRimeSchemaInfo> {
        static constexpr auto kJavaDescriptor { "Licu/astronot233/rime/SchemaInfo;" };
        static facebook::jni::local_ref<JRimeSchemaInfo> create(jstring schemaId, jstring schemaName);
    };

    struct cstr_from_jstring {
        jstring const & jstr;
        char const * const cstr;
        explicit cstr_from_jstring(jstring const &);
        ~cstr_from_jstring();
        operator char const *() const;
        [[nodiscard]] char * copy() const;
    };

    [[nodiscard]] jstring jstring_from_cstr(char const * = "");

}

#endif // RIME_JNI_HPP
