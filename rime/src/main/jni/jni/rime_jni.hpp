#ifndef RIME_JNI_HPP
#define RIME_JNI_HPP

#include <memory>
#include <vector>
#include <string>

#include <jni.h>
#include <librime/rime_api.h>
#include "rime_proto_api.h"

#include <android/log.h>
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,   LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,    LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,    LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,   LOG_TAG, __VA_ARGS__)
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,   LOG_TAG, __VA_ARGS__)

namespace rime::jni {

    constexpr auto MAX_BUFFER_LENGTH { 1ULL << 10ULL };

    std::string stringJavaToCxx(JNIEnv * env, jstring jstr);
    jstring stringCxxToJava(JNIEnv * env, std::string const & str);
    
    struct JRimeSchemaInfo {
        std::string schemaId;
        std::string schemaName;
        // JRimeSchemaInfo(char const * schemaId, char const * schemaName);
    };

    class JRimeCore {
    private:
        RimeApi * const rime { rime_get_api() };
        RimeProtoApi * const proto {
            reinterpret_cast<RimeProtoApi *>(rime_proto_get_api())
        };
        std::string const sharedDataDir;
        std::string const userDataDir;
        std::string const appName;
        RimeSessionId session {};

    public:
        JRimeCore(
            std::string const & sharedDataDir,
            std::string const & userDataDir,
            std::string const & appName
        );
        ~JRimeCore();

// Lifecycle
        Bool startup(Bool fullCheck);
        void shutdown();
        Bool syncUserData();

// IO behavior
        Bool processKey(int keyCode, int mask);
        Bool simulateKeySequence(std::string const & sequence);
        Bool commitComposition();
        void clearComposition();

// Option
        void setOption(std::string const & option, Bool value);
        Bool getOption(std::string const & option);
        void setProperty(std::string const & property, std::string const & value);
        std::string getProperty(std::string const & property);

// Schema
        Bool deploySchema(std::string const & schemaFile);
        std::vector<JRimeSchemaInfo> getSchemaList();
        std::string getCurrentSchemaId();
        Bool selectSchema(std::string const & schemaId);

// Candidate and page
        Bool selectCandidate(int index);
        Bool deleteCandidate(int index);
        Bool highlightCandidate(int index);
        Bool changePage(Bool backward);

// Config
        Bool deployConfigFile(std::string const & fileName, std::string const & versionKey);

// Proto
        std::string getCommitProtoJson();
        std::string getContextProtoJson();
        std::string getStatusProtoJson();

    private:
        RimeSessionId getSession(bool newSession = true);
    };

}

#endif // RIME_JNI_HPP
