#include "rime_jni.hpp"
#include "fbjni_alias.hpp"

namespace rime {

// struct RimeTraitsAndroid : public RimeTraits

    local_ref<JRimeSchemaInfo> JRimeSchemaInfo::create(jstring schemaId, jstring schemaName) {
        return newInstance(schemaId, schemaName);
    }

}