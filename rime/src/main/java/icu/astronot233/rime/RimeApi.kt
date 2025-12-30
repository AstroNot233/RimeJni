package icu.astronot233.rime

class RimeApi {

    companion object {
        @JvmStatic  // Factory method
        external fun create(
            sharedDataDir: String,
            userDataDir: String,
            appName: String,
        ): RimeApi
        init { System.loadLibrary("rime-jni") }
    }
    // Lifecycle
    external fun startup(fullCheck: Boolean = false): Boolean
    external fun shutdown()
    external fun syncUserData(): Boolean
    // IO behavior
    external fun processKey(keyCode: Int, mask: Int = 0): Boolean
    external fun simulateKeySequence(sequence: String): Boolean
    external fun commitComposition(): Boolean
    external fun clearComposition(): Boolean
    // Option
    external fun setOption(option: String, value: Boolean)
    external fun getOption(option: String): String
    external fun setProperty(property: String, value: Boolean)
    external fun getProperty(property: String): String
    // Schema
    external fun deploySchema(schemaFile: String): Boolean
    external fun getSchemaList(): Array<RimeSchemaInfo>
    external fun getCurrentSchemaId(): String
    external fun selectSchema(schemaId: String): Boolean
    // Candidate and page
    external fun selectCandidate(index: Int): Boolean
    external fun deleteCandidate(index: Int): Boolean
    external fun highlightCandidate(index: Int): Boolean
    external fun changePage(backward: Boolean): Boolean
    // Config
    external fun deployConfigFile(fileName: String, versionKey: String): Boolean
    // Proto
    private external fun getCommitProtoJson(): String
    private external fun getContextProtoJson(): String
    private external fun getStatusProtoJson(): String
    val commit get() = Commit.fromJson(getCommitProtoJson())
    val context get() = Context.fromJson(getContextProtoJson())
    val status get() = Status.fromJson(getStatusProtoJson())

}
