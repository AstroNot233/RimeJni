#include "rime_jni.hpp"

extern void rime_require_module_lua();
extern void rime_require_module_predict();

using namespace rime::jni;

static std::unique_ptr<JRimeCore> instance { nullptr };
bool createInstance(std::string const & sharedDir, std::string const & userDir, std::string const & appName) {
    static std::string cachedSharedDir, cachedUserDir, cachedAppName;
    if (instance)
        return (sharedDir == cachedSharedDir && userDir == cachedUserDir && appName == cachedAppName);
    cachedSharedDir = sharedDir;
    cachedUserDir   = userDir;
    cachedAppName   = appName;
    instance = std::make_unique<JRimeCore>(sharedDir, userDir, appName);
    return true;
}
JRimeCore * getInstance() {
    if (!instance)
        throw std::runtime_error("RimeApi is not initialized");
    return instance.get();
}

// Lifecycle
static jboolean initialize_RimeApi(JNIEnv* env, jclass /*class*/,
    jstring sharedDataDir, jstring userDataDir, jstring appName) {
    std::string sD { stringJavaToCxx(env, sharedDataDir) };
    std::string uD { stringJavaToCxx(env, userDataDir) };
    std::string aN { stringJavaToCxx(env, appName) };
    return createInstance(sD, uD, aN) ? JNI_TRUE : JNI_FALSE;
}
static jboolean startup_RimeApi_bool(JNIEnv * /*env*/, jclass /*class*/, jboolean fullCheck) {
    return getInstance()->startup(fullCheck);
}
static void shutdown_RimeApi(JNIEnv* /*env*/, jclass /*class*/) {
    return getInstance()->shutdown();
}
static jboolean syncUserData_RimeApi(JNIEnv* /*env*/, jclass /*class*/) {
    return getInstance()->syncUserData();
}

// IO behavior
static jboolean processKey_RimeApi_int_int(JNIEnv* /*env*/, jclass /*class*/, jint keyCode, jint mask) {
    return getInstance()->processKey(keyCode, mask);
}
static jboolean simulateKeySequence_RimeApi(JNIEnv* env, jclass /*class*/, jstring sequence) {
    std::string s { stringJavaToCxx(env, sequence) };
    return getInstance()->simulateKeySequence(s);
}
static jboolean commitComposition_RimeApi(JNIEnv* /*env*/, jclass /*class*/) {
    return getInstance()->commitComposition();
}
static void clearComposition_RimeApi(JNIEnv* /*env*/, jclass /*class*/) {
    return getInstance()->clearComposition();
}

// Option
static void setOption_RimeApi(JNIEnv* env, jclass /*class*/, jstring option, jboolean value) {
    std::string o { stringJavaToCxx(env, option) };
    return getInstance()->setOption(o, value);
}
static jboolean getOption_RimeApi(JNIEnv* env, jclass /*class*/, jstring option) {
    std::string o { stringJavaToCxx(env, option) };
    return getInstance()->getOption(o);
}
static void setProperty_RimeApi(JNIEnv* env, jclass /*class*/, jstring property, jstring value) {
    std::string p { stringJavaToCxx(env, property) };
    std::string v { stringJavaToCxx(env, value) };
    return getInstance()->setProperty(p, v);
}
static jstring getProperty_RimeApi(JNIEnv* env, jclass /*class*/, jstring property) {
    std::string p { stringJavaToCxx(env, property) };
    std::string v { getInstance()->getProperty(p) };
    return stringCxxToJava(env, v);
}

// Schema
static jboolean deploySchema_RimeApi(JNIEnv* env, jclass /*class*/, jstring schemaFile) {
    std::string sF { stringJavaToCxx(env, schemaFile) };
    return getInstance()->deploySchema(sF);
}
static jobjectArray getSchemaList_RimeApi(JNIEnv* env, jclass /*class*/) {
    jclass className { env->FindClass("icu/astronot233/rime/RimeSchemaInfo") };
    if (!className)
        throw std::runtime_error("RimeSchemaInfo class not found");
    jmethodID factoryMethod { env->GetStaticMethodID(
        className,
        "create", 
        "(Ljava/lang/String;Ljava/lang/String;)Licu/astronot233/rime/RimeSchemaInfo;"
    ) };
    if (!factoryMethod)
        throw std::runtime_error("RimeSchemaInfo.create method not found");
    std::vector<JRimeSchemaInfo> sL { getInstance()->getSchemaList() };
    jobjectArray result { env->NewObjectArray( sL.size(), className, nullptr) };
    if (!result)
        throw std::runtime_error("Failed to create schema list");
    for (size_t i { 0 }; i < sL.size(); ++i) {
        jstring sI { stringCxxToJava(env, sL[i].schemaId) };
        jstring sN { stringCxxToJava(env, sL[i].schemaName) };
        jobject obj { env->CallStaticObjectMethod(className, factoryMethod, sI, sN) };
        env->DeleteLocalRef(sI);
        env->DeleteLocalRef(sN);
        if (!obj)
            throw std::runtime_error("Failed to create schema info");
        env->SetObjectArrayElement(result, i, obj);
        env->DeleteLocalRef(obj);
    }
    env->DeleteLocalRef(className);
    return result;
}
static jstring getCurrentSchemaId_RimeApi(JNIEnv* env, jclass /*class*/) {
    std::string sI { getInstance()->getCurrentSchemaId() };
    return stringCxxToJava(env, sI);
}
static jboolean selectSchema_RimeApi(JNIEnv* env, jclass /*class*/, jstring schemaId) {
    std::string sI { stringJavaToCxx(env, schemaId) };
    return getInstance()->selectSchema(sI);
}

// Candidate and paging
static jboolean selectCandidate_RimeApi(JNIEnv* /*env*/, jclass /*class*/, jint index) {
    return getInstance()->selectCandidate(index);
}
static jboolean deleteCandidate_RimeApi(JNIEnv* /*env*/, jclass /*class*/, jint index) {
    return getInstance()->deleteCandidate(index);
}
static jboolean highlightCandidate_RimeApi(JNIEnv* /*env*/, jclass /*class*/, jint index) {
    return getInstance()->highlightCandidate(index);
}
static jboolean changePage_RimeApi(JNIEnv* /*env*/, jclass /*class*/, jboolean backward) {
    return getInstance()->changePage(backward);
}

// Config
static jboolean deployConfigFile_RimeApi(JNIEnv* env, jclass /*class*/, jstring fileName, jstring versionKey) {
    std::string fN { stringJavaToCxx(env, fileName) };
    std::string vK { stringJavaToCxx(env, versionKey) };
    return getInstance()->deployConfigFile(fN, vK);
}

// Proto
static jstring getCommitProtoJson_RimeApi(JNIEnv* env, jclass /*class*/) {
    std::string cP { getInstance()->getCommitProtoJson() };
    return stringCxxToJava(env, cP);
}
static jstring getContextProtoJson_RimeApi(JNIEnv* env, jclass /*class*/) {
    std::string cP { getInstance()->getContextProtoJson() };
    return stringCxxToJava(env, cP);
}
static jstring getStatusProtoJson_RimeApi(JNIEnv* env, jclass /*class*/) {
    std::string sP { getInstance()->getStatusProtoJson() };
    return stringCxxToJava(env, sP);
}

static JNINativeMethod const methods[] {
    {"initialize", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z", (void*)initialize_RimeApi},
    
    {"startupImpl", "(Z)Z", (void*)startup_RimeApi_bool},
    {"shutdown", "()V", (void*)shutdown_RimeApi},
    {"syncUserData", "()Z", (void*)syncUserData_RimeApi},
    
    {"processKeyImpl", "(II)Z", (void*)processKey_RimeApi_int_int},
    {"simulateKeySequence", "(Ljava/lang/String;)Z", (void*)simulateKeySequence_RimeApi},
    {"commitComposition", "()Z", (void*)commitComposition_RimeApi},
    {"clearComposition", "()V", (void*)clearComposition_RimeApi},
    
    {"setOption", "(Ljava/lang/String;Z)V", (void*)setOption_RimeApi},
    {"getOption", "(Ljava/lang/String;)Z", (void*)getOption_RimeApi},
    {"setProperty", "(Ljava/lang/String;Ljava/lang/String;)V", (void*)setProperty_RimeApi},
    {"getProperty", "(Ljava/lang/String;)Ljava/lang/String;", (void*)getProperty_RimeApi},
    
    {"deploySchema", "(Ljava/lang/String;)Z", (void*)deploySchema_RimeApi},
    {"getSchemaList", "()[Licu/astronot233/rime/RimeSchemaInfo;", (void*)getSchemaList_RimeApi},
    {"getCurrentSchemaId", "()Ljava/lang/String;", (void*)getCurrentSchemaId_RimeApi},
    {"selectSchema", "(Ljava/lang/String;)Z", (void*)selectSchema_RimeApi},
    
    {"selectCandidate", "(I)Z", (void*)selectCandidate_RimeApi},
    {"deleteCandidate", "(I)Z", (void*)deleteCandidate_RimeApi},
    {"highlightCandidate", "(I)Z", (void*)highlightCandidate_RimeApi},
    {"changePage", "(Z)Z", (void*)changePage_RimeApi},
    
    {"deployConfigFile", "(Ljava/lang/String;Ljava/lang/String;)Z", (void*)deployConfigFile_RimeApi},
    
    {"getCommitProtoJson", "()Ljava/lang/String;", (void*)getCommitProtoJson_RimeApi},
    {"getContextProtoJson", "()Ljava/lang/String;", (void*)getContextProtoJson_RimeApi},
    {"getStatusProtoJson", "()Ljava/lang/String;", (void*)getStatusProtoJson_RimeApi},
};

bool registerNativeMethods(JNIEnv * env) {
    jclass className { env->FindClass("icu/astronot233/rime/RimeApi") };
    if (!className)
        return false;
    if (env->RegisterNatives(className, methods, sizeof(methods) / sizeof(methods[0])) < 0)
        return false;
    env->DeleteLocalRef(className);
    return true;
}

extern "C" {

    JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM * vm, void * /*reserved*/) {
        rime_require_module_lua();
        rime_require_module_predict();

        JNIEnv * env { nullptr };
        if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK)
            return JNI_ERR;

        if (!registerNativeMethods(env))
            return JNI_ERR;

        return JNI_VERSION_1_6;
    }

}
