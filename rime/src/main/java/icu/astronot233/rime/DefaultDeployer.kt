package icu.astronot233.rime

import android.content.Context
import java.io.File

object DefaultDeployer {
    fun deploy(context: Context) {
        val rimeDir = context.getExternalFilesDir("rime")!!
        copyAssetsToDir(context, "shared", File(rimeDir, "shared"))
    }
    private fun copyAssetsToDir(context: Context, assetPath: String, targetDir: File) {
        targetDir.mkdirs()
        
        context.assets.list(assetPath)?.forEach { fileName ->
            val targetFile = File(targetDir, fileName)
            if (fileName.contains('.')) {
                context.assets.open("$assetPath/$fileName").use { input ->
                    targetFile.outputStream().use { output ->
                        input.copyTo(output)
                    }
                }
            } else {
                copyAssetsToDir(context, "$assetPath/$fileName", targetFile)
            }
        }
    }
}