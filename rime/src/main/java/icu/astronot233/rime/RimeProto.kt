package icu.astronot233.rime

import kotlinx.serialization.decodeFromString
import kotlinx.serialization.json.Json
import kotlinx.serialization.Serializable
import kotlinx.serialization.SerialName

@Serializable data class Commit private constructor(
    val text: String = "",
) {
    companion object {
        fun fromJson(json: String): Commit = Json.decodeFromString<Commit>(json)
    }
}

@Serializable data class Context private constructor(
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
        fun fromJson(json: String): Context = Json.decodeFromString<Context>(json)
    }
}

@Serializable data class Status private constructor(
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
        fun fromJson(json: String): Status = Json.decodeFromString<Status>(json)
    }
}
