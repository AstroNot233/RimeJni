package icu.astronot233.rime

import kotlinx.coroutines.channels.BufferOverflow
import kotlinx.coroutines.flow.MutableSharedFlow
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.asSharedFlow
import kotlinx.coroutines.flow.asStateFlow
import icu.astronot233.rime.SyncStage

class Rime(sharedDataDir: String, userDataDir: String, appName: String) {
    
    private val dispatcher = RimeDispatcher()
    private val msgFlow = MutableSharedFlow<RimeMessage>(
        replay = 1,
        extraBufferCapacity = 16,
        onBufferOverflow = BufferOverflow.DROP_OLDEST
    )
    private val ctxFlow = MutableStateFlow(ContextProto())
    private val stsFlow = MutableStateFlow(StatusProto())
    private val api = RimeApi.create(sharedDataDir, userDataDir, appName)

    val messageFlow = msgFlow.asSharedFlow()
    val contextFlow = ctxFlow.asStateFlow()
    val statusFlow = stsFlow.asStateFlow()

    // Lifecycle
    fun startup(fullCheck: Boolean = false): Boolean {
        dispatcher.startup()
        return api.startup(fullCheck)
    }
    fun shutdown() {
        api.shutdown()
        dispatcher.shutdown()
    }
    suspend fun deploy(): Boolean = dispatcher.execute {
        msgFlow.emit(RimeMessage.Deploy(DeployStage.Startup))
        api.shutdown()
        if (api.startup(true)) {
            msgFlow.emit(RimeMessage.Deploy(DeployStage.Success))
            true
        } else {
            msgFlow.emit(RimeMessage.Deploy(DeployStage.Unknown))
            false
        }
    }
    suspend fun syncUserData(): Boolean = dispatcher.execute {
        msgFlow.emit(RimeMessage.Sync(SyncStage.Startup))
        if (api.syncUserData()) {
            msgFlow.emit(RimeMessage.Sync(SyncStage.Success))
            true
        } else {
            msgFlow.emit(RimeMessage.Sync(SyncStage.Unknown))
            false
        }
    }

    // IO behavior
    suspend fun processKey(keyCode: Int, mask: Int = 0): Boolean = dispatcher.execute {
        if (api.processKey(keyCode, mask)) {
            ctxFlow.value = api.context
            stsFlow.value = api.status
            if (api.commit.text.isNotEmpty())
                msgFlow.emit(RimeMessage.Commit(api.commit))
            true
        } else {
            false
        }
    }
    suspend fun simulateKeySequence(sequence: String): Boolean = dispatcher.execute {
        if (api.simulateKeySequence(sequence)) {
            ctxFlow.value = api.context
            stsFlow.value = api.status
            if (api.commit.text.isNotEmpty())
                msgFlow.emit(RimeMessage.Commit(api.commit))
            true
        } else {
            false
        }
    }
    suspend fun commitComposition(): Boolean = dispatcher.execute {
        if (api.commitComposition()) {
            ctxFlow.value = api.context
            stsFlow.value = api.status
            msgFlow.emit(RimeMessage.Commit(api.commit))
            true
        } else {
            false
        }
    }
    suspend fun clearComposition() = dispatcher.execute {
        api.clearComposition()
        ctxFlow.value = api.context
        stsFlow.value = api.status
    }

    // Option
    suspend fun setOption(option: String, value: Boolean) = dispatcher.execute {
        api.setOption(option, value)
        stsFlow.value = api.status
    }
    suspend fun getOption(option: String): Boolean = dispatcher.execute {
        api.getOption(option)
    }
    suspend fun setProperty(property: String, value: String) = dispatcher.execute {
        api.setProperty(property, value)
        stsFlow.value = api.status
    }
    suspend fun getProperty(property: String): String = dispatcher.execute {
        api.getProperty(property)
    }

    // Schema
    suspend fun deploySchema(schema: String): Boolean = dispatcher.execute {
        msgFlow.emit(RimeMessage.Deploy(DeployStage.Startup))
        if (api.deploySchema(schema)) {
            msgFlow.emit(RimeMessage.Deploy(DeployStage.Success))
            ctxFlow.value = api.context
            stsFlow.value = api.status
            true
        } else {
            msgFlow.emit(RimeMessage.Deploy(DeployStage.Unknown))
            false
        }
    }
    suspend fun getSchemaList(): Array<RimeSchemaInfo> = dispatcher.execute {
        api.getSchemaList()
    }
    suspend fun getCurrentSchemaId(): String = dispatcher.execute {
        api.getCurrentSchemaId()
    }
    suspend fun selectSchema(schema: String) = dispatcher.execute {
        api.selectSchema(schema)
        ctxFlow.value = api.context
        stsFlow.value = api.status
    }

    // Candidate and page
    suspend fun selectCandidate(index: Int): Boolean = dispatcher.execute {
        if (api.selectCandidate(index)) {
            ctxFlow.value = api.context
            stsFlow.value = api.status
            true
        } else {
            false
        }
    }
    suspend fun deleteCandidate(index: Int): Boolean = dispatcher.execute {
        if (api.deleteCandidate(index)) {
            ctxFlow.value = api.context
            stsFlow.value = api.status
            true
        } else {
            false
        }
    }
    suspend fun highlightCandidate(index: Int): Boolean = dispatcher.execute {
        if (api.highlightCandidate(index)) {
            ctxFlow.value = api.context
            stsFlow.value = api.status
            true
        } else {
            false
        }
    }
    suspend fun changePage(backward: Boolean): Boolean = dispatcher.execute {
        if (api.changePage(backward)) {
            ctxFlow.value = api.context
            stsFlow.value = api.status
            true
        } else {
            false
        }
    }

    // Config
    suspend fun deployConfigFile(fileName: String, versionKey: String): Boolean = dispatcher.execute {
        msgFlow.emit(RimeMessage.Deploy(DeployStage.Startup))
        if (api.deployConfigFile(fileName, versionKey)) {
            msgFlow.emit(RimeMessage.Deploy(DeployStage.Success))
            ctxFlow.value = api.context
            stsFlow.value = api.status
            true
        } else {
            msgFlow.emit(RimeMessage.Deploy(DeployStage.Unknown))
            false
        }
    }

    // Proto
    suspend fun getCommit(): CommitProto = dispatcher.execute { api.commit }
    suspend fun getContext(): ContextProto = dispatcher.execute { api.context }
    suspend fun getStatus(): StatusProto = dispatcher.execute { api.status }

}