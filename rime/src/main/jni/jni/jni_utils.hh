#ifndef JNI_UTILS_HH
#define JNI_UTILS_HH

#include <string>
#include <jni.h>

std::string stringJavaToCxx(JNIEnv * env, jstring jstr);
jstring stringCxxToJava(JNIEnv * env, std::string const & str);

#endif // JNI_UTILS_HH
