package icu.astronot233.rime

import kotlinx.serialization.decodeFromString
import kotlinx.serialization.json.Json
import kotlinx.serialization.Serializable
import kotlinx.serialization.SerialName

@Serializable data class CommitProto(
    val text: String = "",
) {
    companion object {
        fun fromJson(json: String) = Json.decodeFromString<CommitProto>(json)
    }
}

@Serializable data class ContextProto(
    val composition: Composition = Composition(),
    val menu:        Menu        = Menu(),
    val input:       String      = "",
    val caretPos:    Int         = 0,
) {
    @Serializable data class Composition(
        val length:            Int    = 0,
        val cursorPos:         Int    = 0,
        val selStart:          Int    = 0,
        val selEnd:            Int    = 0,
        val preedit:           String = "",
        val commitTextPreview: String = "",
    )
    @Serializable data class Menu(
        val pageSize:                  Int             = 0,
        val pageNumber:                Int             = 0,
        val isLastPage:                Boolean         = false,
        val highlightedCandidateIndex: Int             = 0,
        val candidates:                List<Candidate> = emptyList(),
        val selectKeys:                String          = "",
        val selectLabels:              List<String>    = emptyList()
    )
    @Serializable data class Candidate(
        val text:    String = "",
        val comment: String = "",
        val label:   String = "",
    )
    companion object {
        fun fromJson(json: String) = Json.decodeFromString<ContextProto>(json)
    }
}

@Serializable data class StatusProto(
    val schemaId:    String  = "",
    val disabled:    Boolean = false,
    val composing:   Boolean = false,
    val asciiMode:   Boolean = false,
    val fullShape:   Boolean = false,
    val simplified:  Boolean = false,
    val traditional: Boolean = false,
    val asciiPunct:  Boolean = false,
) {
    companion object {
        fun fromJson(json: String) = Json.decodeFromString<StatusProto>(json)
    }
}
