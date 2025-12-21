#ifndef _RIME_JNI_HPP_
#define _RIME_JNI_HPP_

#include <memory>

#include <fbjni/fbjni.h>
#include <librime/rime_api.h>
#include <librime/rime/key_table.h>

namespace rime {
    void RimeStatus_RegisterNatives();
}

#endif