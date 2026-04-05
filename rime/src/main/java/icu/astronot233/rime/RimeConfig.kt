package icu.astronot233.rime

class RimeConfig private constructor(
    private var peer: Long,
) : AutoCloseable {

    fun getInt(key: String) = getIntImpl(peer, key)

    fun getString(key: String) = getStringImpl(peer, key)

    fun <E : Any> getList(key: String, getAction: RimeConfig.(String) -> E?): List<E> {
        val paths = getListImpl(peer, key)
        val values = ArrayList<E>(paths.size)
        for (path in paths) {
            val value = getAction(this, path)
            if (value == null) {
                continue
            }
            values.add(value)
        }
        return values
    }

    fun setBool(key: String, value: Boolean) = setBoolImpl(peer, key, value)

    override fun close() {
        closeConfig(peer)
    }

    companion object {
        fun openConfig(configId: String): RimeConfig = RimeConfig(openConfigImpl(configId))

        fun openUserConfig(configId: String): RimeConfig = RimeConfig(openUserConfigImpl(configId))

        fun openSchema(schemaId: String): RimeConfig = RimeConfig(openSchemaConfigImpl(schemaId))

        @JvmStatic
        private external fun openConfigImpl(configId: String): Long

        @JvmStatic
        private external fun openUserConfigImpl(configId: String): Long

        @JvmStatic
        private external fun openSchemaConfigImpl(schemaId: String): Long

        @JvmStatic
        private external fun getIntImpl(peer: Long, key: String): Int

        @JvmStatic
        private external fun getStringImpl(peer: Long, key: String): String?

        @JvmStatic
        private external fun getListImpl(peer: Long, key: String): Array<String>

        @JvmStatic
        private external fun setBoolImpl(peer: Long, key: String, value: Boolean)

        @JvmStatic
        private external fun closeConfig(peer: Long)
    }
}
