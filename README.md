# RimeJni
This project is a JNI wrapper for the Chinese input engine [**Rime**](https://rime.im/).

## License
This project is licensed under [MIT LICENSE](LICENSE). For dependencies, please refer to [NOTICE.md](NOTICE.md).

## Build
```bash
git clone https://github.com/AstroNot233/RimeJni.git
cd RimeJni
./gradlew :rime:build
```

## API
The package name is `icu.astronot233.rime`. For a wrapped implementation, use `icu.astronot233.rime.Rime`. For a basic JNI implementation, use `icu.astronot233.rime.RimeApi`.

### Quick Start
```kotlin
import icu.astronot233.rime.Rime

class IME {
    private val rime = Rime
    init {
        rime = Rime ("sharedDataDir", "userDataDir", "appName")
    }
    private fun subscribeToRimeMessage() = rime.messageFlow.onEach { msg ->
        when (msg) {
            is RimeMessage.Deploy -> {}
            is RimeMessage.Commit -> {
                context.commitText(msg.value, 1)
            }
            is RimeMessage.Passby -> {
                val (x11Code, mask) = msg.value
                // Handle keys that rime ignored...
            }
            else -> {}
        }
    }
    fun onCreate() {
        rime.startup(fullCheck = false)
        subscribeToRimeMessage()
    }
    // ...
}
```

### Advanced Usage
Sometimes you may not satisfied with coroutine-based API. You can use `RimeApi` to get a raw JNI implementation.
```kotlin
import icu.astronot233.rime.RimeApi

// ...
    fun updateAll() {
        val commit = RimeApi.getCommit()
        if (commit.isNotEmpty())
            inputConnection.commitText(commit, 1)
        val preedit = RimeApi.getPreedit()
        if (preedit.isNotEmpty())
            inputConnection.setComposingText(preedit, 1)
        val candidates = RimeApi.getCandidates()
        // ...    
    }
    fun commitText() {
        if (!rime.commitComposition())
            return
        // Without coroutines as callback, you need to try updating manually.
        updateAll()
    }
    fun processKeyEvent(keyEvent: KeyEvent) {
        // You need to convert KeyEvent to X11 code and Rime mask.
        // You can use predefined constants by importing `icu.astronot233.rime.X11Keys.*`.
        // Assume that you have already implemented such a function called transform.
        val (x11Code, mask) = transform(keyEvent)
        if (!rime.processKey(x11Code, mask)) {
            // If rime ignored the key, you need to process it.
            // ...
            return
        }
        // Don't forget to update manually.
        updateAll()
    }
```

### API Reference
See [RimeApi.kt](rime/src/main/java/icu/astronot233/rime/RimeApi.kt) for a detailed API reference.
To see predefined constants, see [X11Keys.kt](rime/src/main/java/icu/astronot233/rime/X11Keys/X11Keys.kt).
