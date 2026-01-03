#include "jni_utils.hh"

std::string stringJavaToCxx(JNIEnv * env, jstring jstr) {
    if (!jstr) return {};
    char const * chars { env->GetStringUTFChars(jstr, nullptr) };
    std::string str { chars };
    env->ReleaseStringUTFChars(jstr, chars);
    return str;
}
jstring stringCxxToJava(JNIEnv * env, std::string const & str) {
    return env->NewStringUTF(str.c_str());
}
