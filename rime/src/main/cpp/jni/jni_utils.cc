#include "rime_jni.hpp"
#include "fbjni_alias.hpp"
#include <cstring>

namespace rime::jni {

    cstr_from_jstring::cstr_from_jstring(jstring const & jstr) : jstr { jstr },
        cstr { fbjni::Environment::current()->GetStringUTFChars(jstr, nullptr) } {}
    cstr_from_jstring::~cstr_from_jstring() {
        fbjni::Environment::current()->ReleaseStringUTFChars(jstr, cstr);
    }
    cstr_from_jstring::operator char const * () const { return cstr; }
    [[nodiscard]] char * cstr_from_jstring::copy() const {
        auto const dup { new char[strlen(cstr) + 1] };
        strcpy(dup, cstr);
        return dup;
    }

    [[nodiscard]] jstring jstring_from_cstr(char const * cstr) {
        return fbjni::Environment::current()->NewStringUTF(cstr);
    }

}
