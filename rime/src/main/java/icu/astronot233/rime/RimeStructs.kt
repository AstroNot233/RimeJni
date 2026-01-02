package icu.astronot233.rime

data class RimeSchema(
    val schemaId:   String,
    val schemaName: String,
) {

    companion object {
        @JvmStatic  // Factory method
        fun create(schemaId: String = "", schemaName: String = ""): RimeSchema = RimeSchema(schemaId, schemaName)
    }

}

data class RimeCandidate(
    val text:   String,
    val comment: String,
) {

    companion object {
        @JvmStatic  // Factory method
        fun create(text: String = "", comment: String = ""): RimeCandidate = RimeCandidate(text, comment)
    }

}