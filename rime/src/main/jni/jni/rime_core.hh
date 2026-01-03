#ifndef RIME_CORE_HH
#define RIME_CORE_HH

#include <librime/rime_api.h>

#include <string>
#include <vector>
#include "rime_structs.hh"
#include "rime_session.hh"

namespace rime::jni {

    class JRimeCore {

    private:
        RimeApi * const rime { rime_get_api() };
        std::shared_ptr<SessionTracker> session {};
        std::string const sharedDataDir;
        std::string const userDataDir;
        std::string const appName;

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
        RimeSessionId getSession();

    };

}

#endif // RIME_CORE_HH
