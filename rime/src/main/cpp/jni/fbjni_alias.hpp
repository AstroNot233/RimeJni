#include <fbjni/fbjni.h>

namespace rime {
    namespace fbjni = facebook::jni;

    using fbjni::alias_ref;
    using fbjni::local_ref;
    using fbjni::global_ref;
    using fbjni::weak_ref;

    using fbjni::make_local;
    using fbjni::make_global;
    using fbjni::make_weak;

    using fbjni::JBoolean;
    using fbjni::JByte;
    using fbjni::JCharacter;
    using fbjni::JShort;
    using fbjni::JInteger;
    using fbjni::JLong;
    using fbjni::JFloat;
    using fbjni::JDouble;

    using fbjni::JObject;
    using fbjni::JClass;
    using fbjni::JString;
    using fbjni::JArrayBoolean;
    using fbjni::JArrayByte;
    using fbjni::JArrayChar;
    using fbjni::JArrayShort;
    using fbjni::JArrayInt;
    using fbjni::JArrayLong;
    using fbjni::JArrayFloat;
    using fbjni::JArrayDouble;
    using fbjni::JThrowable;
    using fbjni::JObject;

    template <typename type>
    using jarray = fbjni::JArrayClass<type>;
}