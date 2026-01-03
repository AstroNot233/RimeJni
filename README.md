# RimeJni
This project is a JNI wrapper for the Chinese input engine [**Rime**](https://rime.im/).

## Build
```bash
git clone https://github.com/AstroNot233/RimeJni.git
cd RimeJni
./gradlew :rime:build
```

## API
The package name is `icu.astronot233.rime`. For a wrapped implementation, use `icu.astronot233.rime.Rime`. For basic JNI implementation, use `icu.astronot233.rime.RimeApi`.

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
    fun commitText() {
        // This will clear commit automatically.
        val commit = RimeApi.getCommit()
        if (commit.isNotEmpty())
            context.commitText(commit, 1)
    }
```

## License
This project is licensed under [MIT LICENSE](LICENSE). For dependencies, please refer to [./licenses/README.md](./licenses/README.md).
