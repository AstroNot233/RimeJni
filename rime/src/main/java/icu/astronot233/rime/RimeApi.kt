package icu.astronot233.rime

object RimeApi {
    
    init {
        System.loadLibrary("rime-jni")
    }

    @JvmStatic external fun initialize(
        sharedDataDir: String,
        userDataDir: String,
        appName: String,
    ): Boolean

    // Lifecycle
    @JvmStatic private external fun startupImpl(fullCheck: Boolean): Boolean
    @JvmStatic fun startup(fullCheck: Boolean = false): Boolean = startupImpl(fullCheck)
    @JvmStatic external fun shutdown()
    @JvmStatic external fun syncUserData(): Boolean
    // IO behavior
    @JvmStatic private external fun processKeyImpl(keyCode: Int, mask: Int): Boolean
    @JvmStatic fun processKey(keyCode: Int, mask: Int = 0): Boolean = processKeyImpl(keyCode, mask)
    @JvmStatic external fun simulateKeySequence(sequence: String): Boolean
    @JvmStatic external fun commitComposition(): Boolean
    @JvmStatic external fun clearComposition()
    // Option
    @JvmStatic external fun setOption(option: String, value: Boolean)
    @JvmStatic external fun getOption(option: String): Boolean
    @JvmStatic external fun setProperty(property: String, value: String)
    @JvmStatic external fun getProperty(property: String): String
    // Schema
    @JvmStatic external fun deploySchema(schemaFile: String): Boolean
    @JvmStatic external fun getSchemata(): Array<RimeSchema>
    @JvmStatic external fun getCurrentSchemaId(): String
    @JvmStatic external fun selectSchema(schemaId: String): Boolean
    // Candidate and page
    @JvmStatic external fun getCandidates(): Array<RimeCandidate>
    @JvmStatic external fun selectCandidate(index: Int): Boolean
    @JvmStatic external fun deleteCandidate(index: Int): Boolean
    @JvmStatic external fun highlightCandidate(index: Int): Boolean
    @JvmStatic external fun changePage(backward: Boolean): Boolean
    // Config
    @JvmStatic external fun deployConfigFile(fileName: String, versionKey: String): Boolean
    // Query
    @JvmStatic external fun getCommit(): String
    @JvmStatic external fun getPreedit(): String

}
