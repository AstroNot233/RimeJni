package icu.astronot233.rime

class RimeStatus {
    external fun getSchemaId() : String
    external fun getSchemaName() : String
    external fun isDisabled(): Boolean
    external fun isComposing(): Boolean
    external fun isAsciiMode(): Boolean
    external fun isFullShape(): Boolean
    external fun isSimplified(): Boolean
    external fun isTraditional(): Boolean
    external fun isAsciiPunct(): Boolean
}