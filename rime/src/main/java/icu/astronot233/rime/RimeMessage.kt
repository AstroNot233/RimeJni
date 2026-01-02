package icu.astronot233.rime

enum class MessageType(val value: Int) {
    Unknown(-1), Deploy(1), Sync(2), Commit(3),
    // For debugging only
    KeyEvent(-2),
}

enum class DeployStage(val value: Int) {
    Unknown(-1), Standby(0), Startup(1), Success(2), Halting(4),
}

enum class SyncStage(val value: Int) {
    Unknown(-1), Standby(0), Startup(1), Success(2)
}

sealed class RimeMessage() {

    abstract val type: MessageType
    
    data class Unknown(val value: String): RimeMessage() {
        override val type: MessageType = MessageType.Unknown
    }
    data class Deploy(val value: DeployStage): RimeMessage() {
        override val type: MessageType = MessageType.Deploy
    }
    data class Sync(val value: SyncStage): RimeMessage() {    
        override val type: MessageType = MessageType.Sync
    }
    data class Commit(val value: String): RimeMessage() {
        override val type: MessageType = MessageType.Commit
    }

}
