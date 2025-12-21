#include "rime_jni.hpp"
#include "fbjni_alias.hpp"

namespace rime {

// struct RimeTraitsAndroid : public RimeTraits

    RimeTraitsAndroid::RimeTraitsAndroid(char const * sharedDataDir, char const * userDataDir, char const * distributionVersion, char const * appName) :
        RimeTraits {
            sizeof(RimeTraits) - sizeof(RimeTraits::data_size),
            sharedDataDir, userDataDir,
            "Rime for Android", "astronot233",
            distributionVersion, appName,
            nullptr, 0,
            "", nullptr, nullptr
        } {}

    RimeTraitsAndroid::~RimeTraitsAndroid() {
        delete shared_data_dir;
        delete user_data_dir;
        delete distribution_version;
        delete app_name;
    }

}
