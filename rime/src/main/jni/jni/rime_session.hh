#ifndef RIME_SESSION_HH
#define RIME_SESSION_HH

#include <librime/rime_api.h>

namespace rime::jni {

    class SessionTracker {

    private:
        RimeApi const * const api;
        RimeSessionId const sId;

    public:
        SessionTracker(RimeApi const * api);
        SessionTracker(SessionTracker const &) = delete;
        SessionTracker(SessionTracker&&) = delete;
        ~SessionTracker();
        RimeSessionId id() const;
        operator RimeSessionId() const;
        operator bool() const;

    };

}

#endif // RIME_SESSION_HH
