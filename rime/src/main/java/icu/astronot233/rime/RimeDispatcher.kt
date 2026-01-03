package icu.astronot233.rime

import kotlinx.coroutines.CoroutineDispatcher
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.SupervisorJob
import kotlinx.coroutines.cancel
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import java.util.concurrent.LinkedBlockingQueue
import kotlin.coroutines.CoroutineContext


class RimeDispatcher: CoroutineDispatcher() {

    private val scope = CoroutineScope(this + SupervisorJob())
    private val queue = LinkedBlockingQueue<Runnable>()
    private var thread: Thread? = null
    private val alive get() = (thread?.isAlive ?: false)
    
    fun startup() {
        if (alive) return
        queue.clear()
        thread = Thread(
            {
                try { while (true) queue.take().run() }
                catch (e: InterruptedException) { /* Ignore */ }
            },
            "rime-android"
        ).apply { start() }
    }

    fun shutdown() {
        if (!alive) return
        scope.cancel()
        thread!!.interrupt()
        thread = null
    }
    
    override fun dispatch(context: CoroutineContext, block: Runnable) {
        if (!alive) return
        queue.offer(block)
    }

    suspend fun <Type> execute(block: suspend () -> Type): Type =
        withContext(this) { block() }
    suspend fun launch(block: suspend () -> Unit) =
        scope.launch { block() }

}