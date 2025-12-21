package icu.astronot233.rime

class Rime {
    companion object {
        @JvmStatic
        external fun create(sharedDataDir: String, userDataDir: String, appName: String, callback: NotificationHandler ): Rime
        init { System.loadLibrary("rime-jni") }
    }
    external fun startup(fullCheck: Boolean)
}
