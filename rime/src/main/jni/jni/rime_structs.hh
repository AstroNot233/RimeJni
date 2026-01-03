#ifndef RIME_STRUCTS_HH
#define RIME_STRUCTS_HH

#include <string>

namespace rime::jni {

    constexpr auto MAX_BUFFER_LENGTH { 1ULL << 10ULL };
    constexpr auto MAX_CANDIDATE_COUNT { 1ULL << 7ULL };

    struct JRimeSchema {
        std::string schemaId;
        std::string schemaName;
    };

    struct JRimeCandidate {
        std::string text;
        std::string comment;
    };

}

#endif // RIME_STRUCTS_HH
