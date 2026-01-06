#include "jni_utils.hh"
#include "rime_core.hh"
#include "rime_structs.hh"

extern void rime_require_module_lua();
extern void rime_require_module_predict();

using namespace rime::jni;
using std::string, std::vector;

static std::unique_ptr<JRimeCore> instance { nullptr };
bool createInstance(string const & sharedDir, string const & userDir, string const & appName) {
    static string cachedSharedDir, cachedUserDir, cachedAppName;
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
void dropInstance() {
    instance.reset();
}

// Lifecycle
static jboolean initialize_RimeApi(JNIEnv* env, jclass /*class*/,
    jstring sharedDataDir, jstring userDataDir, jstring appName) {
    string sD { stringJavaToCxx(env, sharedDataDir) };
    string uD { stringJavaToCxx(env, userDataDir) };
    string aN { stringJavaToCxx(env, appName) };
    return createInstance(sD, uD, aN) ? JNI_TRUE : JNI_FALSE;
}
static jboolean startup_RimeApi_bool(JNIEnv* /*env*/, jclass /*class*/, jboolean fullCheck) {
    return getInstance()->startup(fullCheck);
}
static void shutdown_RimeApi(JNIEnv* /*env*/, jclass /*class*/) {
    getInstance()->shutdown();
    return;
}
static jboolean syncUserData_RimeApi(JNIEnv* /*env*/, jclass /*class*/) {
    return getInstance()->syncUserData();
}

// IO behavior
static jboolean processKey_RimeApi_int_int(JNIEnv* /*env*/, jclass /*class*/, jint keyCode, jint mask) {
    return getInstance()->processKey(keyCode, mask);
}
static jboolean simulateKeySequence_RimeApi(JNIEnv* env, jclass /*class*/, jstring sequence) {
    string s { stringJavaToCxx(env, sequence) };
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
    string o { stringJavaToCxx(env, option) };
    return getInstance()->setOption(o, value);
}
static jboolean getOption_RimeApi(JNIEnv* env, jclass /*class*/, jstring option) {
    string o { stringJavaToCxx(env, option) };
    return getInstance()->getOption(o);
}
static void setProperty_RimeApi(JNIEnv* env, jclass /*class*/, jstring property, jstring value) {
    string p { stringJavaToCxx(env, property) };
    string v { stringJavaToCxx(env, value) };
    return getInstance()->setProperty(p, v);
}
static jstring getProperty_RimeApi(JNIEnv* env, jclass /*class*/, jstring property) {
    string p { stringJavaToCxx(env, property) };
    string v { getInstance()->getProperty(p) };
    return stringCxxToJava(env, v);
}

// Schema
static jboolean deploySchema_RimeApi(JNIEnv* env, jclass /*class*/, jstring schemaFile) {
    string sF { stringJavaToCxx(env, schemaFile) };
    return getInstance()->deploySchema(sF);
}
static jobjectArray getSchemata_RimeApi(JNIEnv* env, jclass /*class*/) {
    jclass rimeSchema { env->FindClass("icu/astronot233/rime/RimeSchema") };
    jmethodID create { env->GetStaticMethodID(
        rimeSchema,
        "create", 
        "(Ljava/lang/String;Ljava/lang/String;)Licu/astronot233/rime/RimeSchema;"
    ) };
    vector<JRimeSchema> sL { getInstance()->getSchemata() };
    jobjectArray result { env->NewObjectArray( sL.size(), rimeSchema, nullptr) };
    for (size_t i { 0 }; i < sL.size(); ++i) {
        jstring sI { stringCxxToJava(env, sL[i].schemaId) };
        jstring sN { stringCxxToJava(env, sL[i].schemaName) };
        jobject rS { env->CallStaticObjectMethod(rimeSchema, create, sI, sN) };
        env->DeleteLocalRef(sI);
        env->DeleteLocalRef(sN);
        env->SetObjectArrayElement(result, i, rS);
        env->DeleteLocalRef(rS);
    }
    env->DeleteLocalRef(rimeSchema);
    return result;
}
static jobject getCurrentSchema_RimeApi(JNIEnv* env, jclass /*class*/) {
    auto const schema { getInstance()->getCurrentSchema() };
    jclass rimeSchema { env->FindClass("icu/astronot233/rime/RimeSchema") };
    jmethodID create { env->GetStaticMethodID(
        rimeSchema,
        "create",
        "(Ljava/lang/String;Ljava/lang/String;)Licu/astronot233/rime/RimeSchema;"
    ) };
    
    jstring sI { stringCxxToJava(env, schema.schemaId) };
    jstring sN { stringCxxToJava(env, schema.schemaName) };
    
    return env->NewObject(rimeSchema, create, sI, sN);
}
static jboolean selectSchema_RimeApi(JNIEnv* env, jclass /*class*/, jstring schemaId) {
    string sI { stringJavaToCxx(env, schemaId) };
    return getInstance()->selectSchema(sI);
}

// Candidate and paging
static jobjectArray getCandidates_RimeApi(JNIEnv* env, jclass /*class*/) {
    jclass rimeCandidate { env->FindClass("icu/astronot233/rime/RimeCandidate") };
    jmethodID create { env->GetStaticMethodID(
        rimeCandidate,
        "create",
        "(Ljava/lang/String;Ljava/lang/String;)Licu/astronot233/rime/RimeCandidate;"
    ) };
    vector<JRimeCandidate> cL { getInstance()->getCandidates() };
    jobjectArray result { env->NewObjectArray( cL.size(), rimeCandidate, nullptr) };
    for (size_t i { 0 }; i < cL.size(); ++i) {
        jstring cT { stringCxxToJava(env, cL[i].text) };
        jstring cC { stringCxxToJava(env, cL[i].comment) };
        jobject rC { env->CallStaticObjectMethod(rimeCandidate, create, cT, cC) };
        env->DeleteLocalRef(cT);
        env->DeleteLocalRef(cC);
        env->SetObjectArrayElement(result, i, rC);
        env->DeleteLocalRef(rC);
    }
    env->DeleteLocalRef(rimeCandidate);
    return result;
}
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
    string fN { stringJavaToCxx(env, fileName) };
    string vK { stringJavaToCxx(env, versionKey) };
    return getInstance()->deployConfigFile(fN, vK);
}

// Query
static jint getStatus_RimeApi(JNIEnv* env, jclass /*class*/) {
    return getInstance()->getStatus();
}
static jstring getCommit_RimeApi(JNIEnv* env, jclass /*class*/) {
    return stringCxxToJava(env, getInstance()->getCommit() );
}
static jstring getPreedit_RimeApi(JNIEnv* env, jclass /*class*/) {
    return stringCxxToJava(env, getInstance()->getPreedit() );
}

static JNINativeMethod const methods[] {
    {"initialize", "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)Z", (void*)initialize_RimeApi},
    
    {"startupImpl",  "(Z)Z", (void*)startup_RimeApi_bool},
    {"shutdown",     "()V",  (void*)shutdown_RimeApi},
    {"syncUserData", "()Z",  (void*)syncUserData_RimeApi},
    
    {"processKeyImpl",      "(II)Z",                 (void*)processKey_RimeApi_int_int},
    {"simulateKeySequence", "(Ljava/lang/String;)Z", (void*)simulateKeySequence_RimeApi},
    {"commitComposition",   "()Z",                   (void*)commitComposition_RimeApi},
    {"clearComposition",    "()V",                   (void*)clearComposition_RimeApi},
    
    {"setOption",   "(Ljava/lang/String;Z)V",                  (void*)setOption_RimeApi},
    {"getOption",   "(Ljava/lang/String;)Z",                   (void*)getOption_RimeApi},
    {"setProperty", "(Ljava/lang/String;Ljava/lang/String;)V", (void*)setProperty_RimeApi},
    {"getProperty", "(Ljava/lang/String;)Ljava/lang/String;",  (void*)getProperty_RimeApi},
    
    {"deploySchema",       "(Ljava/lang/String;)Z",                (void*)deploySchema_RimeApi},
    {"getSchemataImpl",    "()[Licu/astronot233/rime/RimeSchema;", (void*)getSchemata_RimeApi},
    {"getCurrentSchema",   "()Licu/astronot233/rime/RimeSchema;",  (void*)getCurrentSchema_RimeApi},
    {"selectSchema",       "(Ljava/lang/String;)Z",                (void*)selectSchema_RimeApi},
    
    {"getCandidatesImpl",  "()[Licu/astronot233/rime/RimeCandidate;", (void*)getCandidates_RimeApi},
    {"selectCandidate",    "(I)Z",                                    (void*)selectCandidate_RimeApi},
    {"deleteCandidate",    "(I)Z",                                    (void*)deleteCandidate_RimeApi},
    {"highlightCandidate", "(I)Z",                                    (void*)highlightCandidate_RimeApi},
    {"changePage",         "(Z)Z",                                    (void*)changePage_RimeApi},
    
    {"deployConfigFile", "(Ljava/lang/String;Ljava/lang/String;)Z", (void*)deployConfigFile_RimeApi},
    
    {"getStatusImpl", "()I",                  (void*)getStatus_RimeApi},
    {"getCommit",     "()Ljava/lang/String;", (void*)getCommit_RimeApi},
    {"getPreedit",    "()Ljava/lang/String;", (void*)getPreedit_RimeApi}
};

bool registerNativeMethods(JNIEnv* env) {
    jclass className { env->FindClass("icu/astronot233/rime/RimeApi") };
    if (!className)
        return false;
    if (env->RegisterNatives(className, methods, sizeof(methods) / sizeof(methods[0])) < 0)
        return false;
    env->DeleteLocalRef(className);
    return true;
}

extern "C" {

    JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* /*reserved*/) {
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
