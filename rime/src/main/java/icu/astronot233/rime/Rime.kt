package icu.astronot233.rime

import android.view.KeyEvent
import android.util.Log
import kotlinx.coroutines.channels.BufferOverflow
import kotlinx.coroutines.flow.MutableSharedFlow
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.asSharedFlow
import kotlinx.coroutines.flow.asStateFlow
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
    private val pedFlow = MutableStateFlow(String())
    private val cddFlow = MutableStateFlow(emptyList<RimeCandidate>())
    private val sttFlow = MutableStateFlow(RimeStatus(0))
    
    val messageFlow = msgFlow.asSharedFlow()
    val preeditFlow = pedFlow.asStateFlow()
    val candidatesFlow = cddFlow.asStateFlow()
    val statusFlow = sttFlow.asStateFlow()

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
    suspend fun processX11Code(x11Code: Int, mask: Int = 0): Boolean = dispatcher.execute {
        Log.d("Rime", "processX11Code: ($x11Code, $mask)")
        if (RimeApi.processKey(x11Code, mask)) {
            val commit = RimeApi.getCommit()
            val preedit = RimeApi.getPreedit()
            val candidates = RimeApi.getCandidates()
            if (commit.isNotEmpty())
                msgFlow.emit(RimeMessage.Commit(commit))
            pedFlow.value = preedit
            cddFlow.value = candidates
            true
        } else {
            msgFlow.emit(RimeMessage.Passby(Pair(x11Code, mask)))
            false
        }
    }
    suspend fun processCodePoint(codePoint: Int, mask: Int = 0): Boolean {
        val x11Code = KeyMapper.toX11Key(codePoint)
        return processX11Code(x11Code, mask)
    }
    suspend fun processKeyEvent(keyEvent: KeyEvent): Boolean {
        val (x11Code, mask) = KeyMapper.toRimeStyled(keyEvent)
        return processX11Code(x11Code, mask)
    }
    suspend fun simulateKeySequence(sequence: String): Boolean = dispatcher.execute {
        if (RimeApi.simulateKeySequence(sequence)) {
            val commit = RimeApi.getCommit()
            val preedit = RimeApi.getPreedit()
            val candidates = RimeApi.getCandidates()
            if (commit.isNotEmpty())
                msgFlow.emit(RimeMessage.Commit(commit))
            pedFlow.value = preedit
            cddFlow.value = candidates
            true
        } else {
            false
        }
    }
    suspend fun commitComposition(): Boolean = dispatcher.execute {
        if (RimeApi.commitComposition()) {
            val commit = RimeApi.getCommit()
            val preedit = RimeApi.getPreedit()
            val candidates = RimeApi.getCandidates()
            msgFlow.emit(RimeMessage.Commit(commit))
            pedFlow.value = preedit
            cddFlow.value = candidates
            true
        } else {
            false
        }
    }
    suspend fun clearComposition() = dispatcher.execute {
        RimeApi.clearComposition()
        pedFlow.value = RimeApi.getPreedit()
        cddFlow.value = RimeApi.getCandidates()
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
    suspend fun getSchemata(): List<RimeSchema> = dispatcher.execute {
        RimeApi.getSchemata()
    }
    suspend fun getCurrentSchema(): RimeSchema = dispatcher.execute {
        RimeApi.getCurrentSchema()
    }
    suspend fun selectSchema(schema: String) = dispatcher.execute {
        RimeApi.selectSchema(schema)
    }

    // Candidate and page
    suspend fun getCandidates(): List<RimeCandidate> = dispatcher.execute {
        RimeApi.getCandidates()
    }
    suspend fun selectCandidate(index: Int): Boolean = dispatcher.execute {
        if (RimeApi.selectCandidate(index)) {
            val commit = RimeApi.getCommit()
            val preedit = RimeApi.getPreedit()
            val candidates = RimeApi.getCandidates()
            if (commit.isNotEmpty())
                msgFlow.emit(RimeMessage.Commit(commit))
            pedFlow.value = preedit
            cddFlow.value = candidates
            true
        } else {
            false
        }
    }
    suspend fun deleteCandidate(index: Int): Boolean = dispatcher.execute {
        if (RimeApi.deleteCandidate(index)) {
            val candidates = RimeApi.getCandidates()
            cddFlow.value = candidates
            true
        } else {
            false
        }
    }
    suspend fun highlightCandidate(index: Int): Boolean = dispatcher.execute {
        if (RimeApi.highlightCandidate(index)) {
            val candidates = RimeApi.getCandidates()
            cddFlow.value = candidates
            true
        } else {
            false
        }
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
    suspend fun getStatus(): RimeStatus = dispatcher.execute {
        RimeApi.getStatus()
    }
    suspend fun getCommit(): String = dispatcher.execute {
        RimeApi.getCommit()
    }
    suspend fun getPreedit(): String = dispatcher.execute {
        RimeApi.getPreedit()
    }

}