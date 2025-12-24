package icu.astronot233.rime

enum class RimeMessageType(val value: Int) {
    Unknown(0), Schema(1), Option(2), Deploy(3);
}

enum class RimeDeployStage(val value: Int) {
    Failure(-1), Success(0), Start(1);
}

sealed class RimeMessage {
    abstract val type: RimeMessageType
    data class Unknown(val value: String, override val type: RimeMessageType = RimeMessageType.Unknown): RimeMessage()
    data class Schema(val value: String, override val type: RimeMessageType = RimeMessageType.Schema): RimeMessage()
    data class Option(val value: Boolean, override val type: RimeMessageType = RimeMessageType.Option): RimeMessage()
    data class Deploy(val value: RimeDeployStage, override val type: RimeMessageType = RimeMessageType.Deploy): RimeMessage()
    companion object {
        fun create(messageType: String, messageValue: String): RimeMessage = when(messageType) {
            "schema" -> Schema(messageValue)
            "option" -> when (messageValue) {
                "ascii_mode" -> Option(true)
                "!ascii_mode" -> Option(false)
                else -> throw IllegalArgumentException("Unknown RimeMessage: type: Option, value: $messageValue.")
            }
            "deploy" -> when (messageValue) {
                "failure" -> Deploy(RimeDeployStage.Failure)
                "success" -> Deploy(RimeDeployStage.Success)
                else -> throw IllegalArgumentException("Unknown RimeMessage: type: Deploy, value: $messageValue.")
            }
            else -> Unknown(messageValue)
        }
    }
}

class NotificationHandler {
    fun handle(messageType: String, messageValue: String) {
//        val msg = RimeMessage.create(messageType, messageValue)
//        when (msg) {
//            is RimeMessage.Schema ->
//        }
    }
}