#include "rime_jni.hpp"

namespace rime::jni {

    using std::string;

    string stringJavaToCxx(JNIEnv * env, jstring jstr) {
        if (!jstr) return {};
        char const * chars { env->GetStringUTFChars(jstr, nullptr) };
        string str { chars };
        env->ReleaseStringUTFChars(jstr, chars);
        return str;
    }

    jstring stringCxxToJava(JNIEnv * env, string const & str) {
        return env->NewStringUTF(str.c_str());
    }

}
