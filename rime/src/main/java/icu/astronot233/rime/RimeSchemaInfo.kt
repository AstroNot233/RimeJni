package icu.astronot233.rime

class RimeSchemaInfo private constructor(val schemaId: String, val schemaName: String) {

    companion object {
        @JvmStatic  // Factory method
        fun create(schemaId: String = "", schemaName: String = ""): RimeSchemaInfo = RimeSchemaInfo(schemaId, schemaName)
    }

}