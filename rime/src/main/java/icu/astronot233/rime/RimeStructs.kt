package icu.astronot233.rime

data class RimeSchema(
    val schemaId:   String,
    val schemaName: String,
) {

    companion object {
        @JvmStatic  // Factory method
        fun create(schemaId: String, schemaName: String): RimeSchema = RimeSchema(schemaId, schemaName)
    }

}

data class RimeCandidate(
    val text:    String,
    val comment: String,
) {

    companion object {
        @JvmStatic  // Factory method
        fun create(text: String, comment: String): RimeCandidate = RimeCandidate(text, comment)
    }

}

data class RimeStatus(
    private val status: Int,
) {

    val disabled       : Boolean get() = (status and 0x01) != 0
    val composing      : Boolean get() = (status and 0x02) != 0
    val asciiMode      : Boolean get() = (status and 0x04) != 0
    val fullShape      : Boolean get() = (status and 0x08) != 0
    val simplification : Boolean get() = (status and 0x10) != 0
    val traditional    : Boolean get() = (status and 0x20) != 0
    val asciiPunct     : Boolean get() = (status and 0x40) != 0

    override fun toString(): String = """{
        \"disabled\"       : $disabled,
        \"composing\"      : $composing,
        \"asciiMode\"      : $asciiMode,
        \"fullShape\"      : $fullShape,
        \"simplification\" : $simplification,
        \"traditional\"    : $traditional,
        \"asciiPunct\"     : $asciiPunct,
    }""".trimIndent()

}
