package icu.astronot233.rime

import kotlinx.coroutines.channels.BufferOverflow
import kotlinx.coroutines.flow.MutableSharedFlow
import kotlinx.coroutines.flow.asSharedFlow

class Rime(sharedDataDir: String, userDataDir: String, appName: String): RimeNotificationHandler {
    
    
    private val dispatcher = RimeDispatcher()
    private val msgFlow = MutableSharedFlow<RimeMessage>(
        extraBufferCapacity = 16,
        onBufferOverflow = BufferOverflow.DROP_OLDEST
    )
    val messageFlow = msgFlow.asSharedFlow()

    init {
    }
    fun startup(fullCheck: Boolean = false) = core.startup(fullCheck)
    fun shutdown() {
        dispatcher.shutdown()
        core.shutdown()
    }

    suspend fun deploy(): Unit = dispatcher.execute {
        core.shutdown()
        core.startup(true)
    }
    suspend fun syncUserData() = dispatcher.launch {
        core.syncUserData()
    }

    // Candidate
    suspend fun selectCandidate(index: Int): Boolean = dispatcher.execute {
        core.selectCandidate(index).apply {
            if (this) {
                val commitText = core.getRimeComposition()
            }
        }
    }

    // private:

    private val core = RimeCore.create(sharedDataDir, userDataDir, appName, this)

    private var schema = RimeSchemaInfo.create()
    // private var status = 

    override fun handleRimeNotification(messageType: String, messageValue: String): Boolean {
        val msg = RimeMessage.create(messageType, messageValue)
        when (msg.type) {
            RimeMessageType.Schema -> {}
            else -> {}
        }
        return true
    }

}