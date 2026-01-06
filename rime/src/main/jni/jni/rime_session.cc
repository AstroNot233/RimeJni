#include "rime_session.hh"
#include "debug/logger.hh"

#include <stdexcept>

namespace rime::jni {

    SessionTracker::SessionTracker(RimeApi const * api) :
    api { api }, sId { api->create_session() } {
        LOGV("New session: 0x%lX", sId);
        if (!operator bool()) throw std::runtime_error("Failed to create session");
    }
    SessionTracker::~SessionTracker() {
        if (operator bool()) api->destroy_session(sId);
    }
    RimeSessionId SessionTracker::id() const { return sId; }
    SessionTracker::operator RimeSessionId() const { return sId; }
    SessionTracker::operator bool() const { return sId && api->find_session(sId); }

}