package icu.astronot233.rime

import kotlinx.coroutines.channels.BufferOverflow
import kotlinx.coroutines.flow.MutableSharedFlow
import kotlinx.coroutines.flow.asSharedFlow
import icu.astronot233.rime.SyncStage
import icu.astronot233.rime.RimeApi

class Rime(sharedDataDir: String, userDataDir: String, appName: String) {

    init {
        RimeApi.initialize(sharedDataDir, userDataDir, appName)
    }
    
    private val dispatcher = RimeDispatcher()
    private val msgFlow = MutableSharedFlow<RimeMessage>(
        replay = 1,
        extraBufferCapacity = 16,
        onBufferOverflow = BufferOverflow.DROP_OLDEST
    )

    val messageFlow = msgFlow.asSharedFlow()

    // Lifecycle
    fun startup(fullCheck: Boolean = false): Boolean {
        dispatcher.startup()
        return RimeApi.startup(fullCheck)
    }
    fun shutdown() {
        RimeApi.shutdown()
        dispatcher.shutdown()
    }
    suspend fun deploy(): Boolean = dispatcher.execute {
        msgFlow.emit(RimeMessage.Deploy(DeployStage.Startup))
        RimeApi.shutdown()
        if (RimeApi.startup(true)) {
            msgFlow.emit(RimeMessage.Deploy(DeployStage.Success))
            true
        } else {
            msgFlow.emit(RimeMessage.Deploy(DeployStage.Unknown))
            false
        }
    }
    suspend fun syncUserData(): Boolean = dispatcher.execute {
        msgFlow.emit(RimeMessage.Sync(SyncStage.Startup))
        if (RimeApi.syncUserData()) {
            msgFlow.emit(RimeMessage.Sync(SyncStage.Success))
            true
        } else {
            msgFlow.emit(RimeMessage.Sync(SyncStage.Unknown))
            false
        }
    }

    // IO behavior
    suspend fun processKey(keyCode: Int, mask: Int = 0): Boolean = dispatcher.execute {
        if (RimeApi.processKey(keyCode, mask)) {
            if (RimeApi.getCommit().isNotEmpty())
                msgFlow.emit(RimeMessage.Commit(RimeApi.getCommit()))
            true
        } else {
            false
        }
    }
    suspend fun simulateKeySequence(sequence: String): Boolean = dispatcher.execute {
        if (RimeApi.simulateKeySequence(sequence)) {
            if (RimeApi.getCommit().isNotEmpty())
                msgFlow.emit(RimeMessage.Commit(RimeApi.getCommit()))
            true
        } else {
            false
        }
    }
    suspend fun commitComposition(): Boolean = dispatcher.execute {
        if (RimeApi.commitComposition()) {
            msgFlow.emit(RimeMessage.Commit(RimeApi.getCommit()))
            true
        } else {
            false
        }
    }
    suspend fun clearComposition() = dispatcher.execute {
        RimeApi.clearComposition()
    }

    // Option
    suspend fun setOption(option: String, value: Boolean) = dispatcher.execute {
        RimeApi.setOption(option, value)
    }
    suspend fun getOption(option: String): Boolean = dispatcher.execute {
        RimeApi.getOption(option)
    }
    suspend fun setProperty(property: String, value: String) = dispatcher.execute {
        RimeApi.setProperty(property, value)
    }
    suspend fun getProperty(property: String): String = dispatcher.execute {
        RimeApi.getProperty(property)
    }

    // Schema
    suspend fun deploySchema(schema: String): Boolean = dispatcher.execute {
        msgFlow.emit(RimeMessage.Deploy(DeployStage.Startup))
        if (RimeApi.deploySchema(schema)) {
            msgFlow.emit(RimeMessage.Deploy(DeployStage.Success))
            true
        } else {
            msgFlow.emit(RimeMessage.Deploy(DeployStage.Unknown))
            false
        }
    }
    suspend fun getSchemata(): Array<RimeSchema> = dispatcher.execute {
        RimeApi.getSchemata()
    }
    suspend fun getCurrentSchemaId(): String = dispatcher.execute {
        RimeApi.getCurrentSchemaId()
    }
    suspend fun selectSchema(schema: String) = dispatcher.execute {
        RimeApi.selectSchema(schema)
    }

    // Candidate and page
    suspend fun selectCandidate(index: Int): Boolean = dispatcher.execute {
        RimeApi.selectCandidate(index)
    }
    suspend fun deleteCandidate(index: Int): Boolean = dispatcher.execute {
        RimeApi.deleteCandidate(index)
    }
    suspend fun highlightCandidate(index: Int): Boolean = dispatcher.execute {
        RimeApi.highlightCandidate(index)
    }
    suspend fun changePage(backward: Boolean): Boolean = dispatcher.execute {
        RimeApi.changePage(backward)
    }

    // Config
    suspend fun deployConfigFile(fileName: String, versionKey: String): Boolean = dispatcher.execute {
        msgFlow.emit(RimeMessage.Deploy(DeployStage.Startup))
        if (RimeApi.deployConfigFile(fileName, versionKey)) {
            msgFlow.emit(RimeMessage.Deploy(DeployStage.Success))
            true
        } else {
            msgFlow.emit(RimeMessage.Deploy(DeployStage.Unknown))
            false
        }
    }

    // Query
    suspend fun getCommit(): String = dispatcher.execute { RimeApi.getCommit() }
    suspend fun getPreedit(): String = dispatcher.execute { RimeApi.getPreedit() }

}