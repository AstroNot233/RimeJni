#ifndef RIME_CORE_HH
#define RIME_CORE_HH

#include <librime/rime_api.h>

#include <string>
#include <vector>
#include "rime_structs.hh"
#include "rime_session.hh"

namespace rime::jni {

    static char const * modules[] =      { "lua", "predict" };
    static char distribution_name[]      { "Rime for Android" };
    static char distribution_code_name[] { "Android" };
    static char distribution_version[]   { RIME_VERSION };

    class JRimeCore {

    private:
        RimeApi * const rime { rime_get_api() };
        std::string const sharedDataDir;
        std::string const userDataDir;
        std::string const appName;
        RimeTraits traits {};

    public:
        JRimeCore(
            std::string const & sharedDataDir,
            std::string const & userDataDir,
            std::string const & appName
        ) noexcept;
        ~JRimeCore() noexcept;

    // Lifecycle
        Bool startup(Bool fullCheck) noexcept;
        void shutdown() noexcept;
        Bool syncUserData() noexcept;

    // IO behavior
        Bool processKey(int keyCode, int mask) noexcept;
        Bool simulateKeySequence(std::string const & sequence) noexcept;
        Bool commitComposition() noexcept;
        void clearComposition() noexcept;

    // Option
        void setOption(std::string const & option, Bool value) noexcept;
        Bool getOption(std::string const & option) noexcept;
        void setProperty(std::string const & property, std::string const & value) noexcept;
        std::string getProperty(std::string const & property) noexcept;

    // Schema
        Bool deploySchema(std::string const & schemaFile) noexcept;
        std::vector<JRimeSchema> getSchemata() noexcept;
        JRimeSchema getCurrentSchema() noexcept;
        Bool selectSchema(std::string const & schemaId) noexcept;

    // Candidate and page
        std::vector<JRimeCandidate> getCandidates() noexcept;
        Bool selectCandidate(int index) noexcept;
        Bool deleteCandidate(int index) noexcept;
        Bool highlightCandidate(int index) noexcept;
        Bool changePage(Bool backward) noexcept;

    // Config
        Bool deployConfigFile(std::string const & fileName, std::string const & versionKey) noexcept;

    // Query
        int getStatus() noexcept;
        std::string getCommit() noexcept;
        JRimePreedit getPreedit() noexcept;

    private:
        RimeSessionId getSessionId() noexcept;

    };

}

#endif // RIME_CORE_HH
