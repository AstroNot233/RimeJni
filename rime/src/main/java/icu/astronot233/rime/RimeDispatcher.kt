package icu.astronot233.rime

import kotlinx.coroutines.CoroutineDispatcher
import kotlinx.coroutines.CoroutineScope
import kotlinx.coroutines.Job
import kotlinx.coroutines.SupervisorJob
import kotlinx.coroutines.cancel
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import java.util.concurrent.LinkedBlockingQueue
import kotlin.coroutines.CoroutineContext


class RimeDispatcher: CoroutineDispatcher() {

    private val scope = CoroutineScope(this + SupervisorJob())
    private val queue = LinkedBlockingQueue<Runnable>()
    private val thread = Thread(
        {
            try {
                while (true) queue.take().run()
            } catch (e: InterruptedException) {
                // Ignore
            }
        },
        "rime-android"
    ).apply { start() }
    
    fun shutdown() {
        scope.cancel()
        thread.interrupt()
    }
    
    override fun dispatch(context: CoroutineContext, block: Runnable) {
        queue.offer(block)
    }

    suspend fun <Type> execute(block: suspend () -> Type): Type =
        withContext(this) { block() }
    suspend fun launch(block: suspend () -> Unit): Job =
        scope.launch { block() }

}