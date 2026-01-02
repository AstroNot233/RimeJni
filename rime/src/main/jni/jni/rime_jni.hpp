#ifndef RIME_JNI_HPP
#define RIME_JNI_HPP

#include <vector>
#include <string>

#include "debug/logger.hpp"

#include <librime/rime_api.h>

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

    class JRimeCore {
    private:
        RimeApi * const rime { rime_get_api() };
        std::string const sharedDataDir;
        std::string const userDataDir;
        std::string const appName;
        RimeSessionId session {};

    public:
        JRimeCore(
            std::string const & sharedDataDir,
            std::string const & userDataDir,
            std::string const & appName
        );
        ~JRimeCore();

// Lifecycle
        Bool startup(Bool fullCheck);
        void shutdown();
        Bool syncUserData();

// IO behavior
        Bool processKey(int keyCode, int mask);
        Bool simulateKeySequence(std::string const & sequence);
        Bool commitComposition();
        void clearComposition();

// Option
        void setOption(std::string const & option, Bool value);
        Bool getOption(std::string const & option);
        void setProperty(std::string const & property, std::string const & value);
        std::string getProperty(std::string const & property);

// Schema
        Bool deploySchema(std::string const & schemaFile);
        std::vector<JRimeSchema> getSchemata();
        std::string getCurrentSchemaId();
        Bool selectSchema(std::string const & schemaId);

// Candidate and page
        std::vector<JRimeCandidate> getCandidates();
        Bool selectCandidate(int index);
        Bool deleteCandidate(int index);
        Bool highlightCandidate(int index);
        Bool changePage(Bool backward);

// Config
        Bool deployConfigFile(std::string const & fileName, std::string const & versionKey);

// Query
        std::string getCommit();
        std::string getPreedit();

    private:
        RimeSessionId getSession(bool newSession = true);
    };

}

#endif // RIME_JNI_HPP
