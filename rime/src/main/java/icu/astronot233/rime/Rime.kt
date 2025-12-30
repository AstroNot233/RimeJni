package icu.astronot233.rime

import kotlinx.coroutines.channels.BufferOverflow
import kotlinx.coroutines.flow.MutableSharedFlow
import kotlinx.coroutines.flow.asSharedFlow

abstract class Rime(sharedDataDir: String, userDataDir: String, appName: String) {
    
    private val dispatcher = RimeDispatcher()
    private val msgFlow = MutableSharedFlow<RimeMessage>(
        extraBufferCapacity = 16,
        onBufferOverflow = BufferOverflow.DROP_OLDEST
    )
    val messageFlow = msgFlow.asSharedFlow()

    init {
    }

    // Lifecycle
    fun startup(fullCheck: Boolean = false) = api.startup(fullCheck)
    fun shutdown() {
        dispatcher.shutdown()
        api.shutdown()
    }

    suspend fun deploy(): Unit = dispatcher.execute {
        api.shutdown()
        api.startup(true)
    }
    suspend fun syncUserData() = dispatcher.launch {
        api.syncUserData()
    }

    // Candidate
    suspend fun selectCandidate(index: Int): Boolean = dispatcher.execute {
        api.selectCandidate(index)
    }

    // Proto
    val commit get() = api.commit
    val context get() = api.context
    val status get() = api.status

    // private:

    private val api = RimeApi.create(sharedDataDir, userDataDir, appName)

    private var schema = RimeSchemaInfo.create()

}