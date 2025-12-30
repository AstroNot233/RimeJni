package icu.astronot233.rime

enum class RimeMessageType(val value: Int) {
    Unknown(0), Schema(1), Option(2), Deploy(3), Commit(4);
}

enum class RimeDeployStage(val value: Int) {
    Failure(-1), Success(0), Start(1);
}

sealed class RimeMessage {

    abstract val type: RimeMessageType
//     data class Unknown(val value: String, override val type: RimeMessageType = RimeMessageType.Unknown): RimeMessage()
//     data class Schema(val value: String, override val type: RimeMessageType = RimeMessageType.Schema): RimeMessage()
//     data class Option(val value: Boolean, override val type: RimeMessageType = RimeMessageType.Option): RimeMessage()
//     data class Deploy(val value: RimeDeployStage, override val type: RimeMessageType = RimeMessageType.Deploy): RimeMessage()
//     data class Commit(val value: String, override val type: RimeMessageType = RimeMessageType.Commit): RimeMessage()
//     companion object {
//         @JvmStatic // Factory method
//         fun create(messageType: String, messageValue: String): RimeMessage = when(messageType) {
//             "schema" -> Schema(messageValue)
//             "option" -> when (messageValue) {
//                 "ascii_mode" -> Option(true)
//                 "!ascii_mode" -> Option(false)
//                 else -> Unknown("Unknown message -> [ $messageType : $messageValue ]")
//             }
//             "deploy" -> when (messageValue) {
//                 "failure" -> Deploy(RimeDeployStage.Failure)
//                 "success" -> Deploy(RimeDeployStage.Success)
//                 "start" -> Deploy(RimeDeployStage.Start)
//                 else -> Unknown("Unknown message -> [ $messageType : $messageValue ]")
//             }
//             "commit" -> Commit(messageValue)
//             else -> Unknown("Unknown message -> [ $messageType : $messageValue ]")
//         }
//     }

}
