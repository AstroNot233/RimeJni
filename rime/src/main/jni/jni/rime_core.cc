#include "rime_core.hh"
#include "debug/logger.hh"

#include <librime/rime/service.h>
#include <librime/rime/context.h>
#include "rime_session.hh"

namespace rime::jni {

    using std::string, std::vector;

// Lifecycle
    Bool JRimeCore::startup(Bool fullCheck) {
        LOGV("startup(fullCheck = %d)", fullCheck);
        return rime->start_maintenance(fullCheck);
    }
    void JRimeCore::shutdown() {
        LOGV("shutdown()");
        session = 0u;
        return rime->finalize();
    }
    Bool JRimeCore::syncUserData() {
        LOGV("syncUserData()");
        return rime->sync_user_data();
    }

// IO behavior
    Bool JRimeCore::processKey(int keyCode, int mask) {
        LOGV("processKey(keyCode = %d, mask = %d)", keyCode, mask);
        return rime->process_key(getSession(), keyCode, mask);
    }
    Bool JRimeCore::simulateKeySequence(string const & sequence) {
        LOGV("simulateKeySequence(sequence = %s)", sequence.c_str());
        return rime->simulate_key_sequence(getSession(), sequence.c_str());
    }
    Bool JRimeCore::commitComposition() {
        LOGV("commitComposition()");
        return rime->commit_composition(getSession());
    }
    void JRimeCore::clearComposition() {
        LOGV("clearComposition()");
        return rime->clear_composition(getSession());
    }

// Option
    void JRimeCore::setOption(string const & option, Bool value) {
        LOGV("setOption(option = %s, value = %d)", option.c_str(), value);
        return rime->set_option(getSession(), option.c_str(), value);
    }
    Bool JRimeCore::getOption(string const & option) {
        LOGV("getOption(option = %s)", option.c_str());
        return rime->get_option(getSession(), option.c_str());
    }
    void JRimeCore::setProperty(string const & property, string const & value) {
        LOGV("setProperty(property = %s, value = %s)", property.c_str(), value.c_str());
        return rime->set_property(getSession(), property.c_str(), value.c_str());
    }
    string JRimeCore::getProperty(string const & property) {
        LOGV("getProperty(property = %s)", property.c_str());
        char value[MAX_BUFFER_LENGTH];
        if (rime->get_property(getSession(), property.c_str(), value, MAX_BUFFER_LENGTH)) {
            return value;
        } else {
            return "";
        }
    }

// Schema
    Bool JRimeCore::deploySchema(string const & schemaFile) {
        LOGV("deploySchema(schemaFile = %s)", schemaFile.c_str());
        return rime->deploy_schema(schemaFile.c_str());
    }
    vector<JRimeSchema> JRimeCore::getSchemata() {
        LOGV("getSchemata()");
        RimeSchemaList list {};
        if (!rime->get_schema_list(&list))
            return vector<JRimeSchema>(0);
        vector<JRimeSchema> rimeSchemaList {};
        rimeSchemaList.reserve(list.size);
        for (size_t i { 0U }; i < list.size; ++i) {
            rimeSchemaList.emplace_back(JRimeSchema {
                list.list[i].schema_id,
                list.list[i].name
            });
        }
        rime->free_schema_list(&list);
        return rimeSchemaList;
    }
    string JRimeCore::getCurrentSchemaId() {
        LOGV("getCurrentSchemaId()");
        char currentSchemaId[MAX_BUFFER_LENGTH];
        if (rime->get_current_schema(getSession(), currentSchemaId, MAX_BUFFER_LENGTH)) {
            return currentSchemaId;
        } else {
            return "";
        }
    }
    Bool JRimeCore::selectSchema(string const & schemaId) {
        LOGV("selectSchema(schemaId = %s)", schemaId.c_str());
        return rime->select_schema(getSession(), schemaId.c_str());
    }

// Candidate and page
    vector<JRimeCandidate> JRimeCore::getCandidates() {
        LOGV("getCandidates()");
        RimeCandidateListIterator iter {};
        if (!rime->candidate_list_from_index(getSession(), &iter, 0))
            return vector<JRimeCandidate>(0);
        vector<JRimeCandidate> rimeCandidateList {};
        rimeCandidateList.reserve(MAX_CANDIDATE_COUNT);
        for (size_t i { 0U }; i < MAX_CANDIDATE_COUNT && rime->candidate_list_next(&iter); ++i)
            rimeCandidateList.emplace_back(JRimeCandidate {
                iter.candidate.text,
                iter.candidate.comment
        });
        rime->candidate_list_end(&iter);
        return rimeCandidateList;
    }
    Bool JRimeCore::selectCandidate(int index) {
        LOGV("selectCandidate(index = %d)", index);
        return rime->select_candidate(getSession(), index);
    }
    Bool JRimeCore::deleteCandidate(int index) {
        LOGV("deleteCandidate(index = %d)", index);
        return rime->delete_candidate(getSession(), index);
    }
    Bool JRimeCore::highlightCandidate(int index) {
        LOGV("highlightCandidate(index = %d)", index);
        return rime->highlight_candidate(getSession(), index);
    }
    Bool JRimeCore::changePage(Bool backward) {
        LOGV("changePage(backward = %d)", backward);
        return rime->change_page(getSession(), backward);
    }

// Config
    Bool JRimeCore::deployConfigFile(string const & fileName, string const & versionKey) {
        LOGV("deployConfigFile(fileName = %s, versionKey = %s)", fileName.c_str(), versionKey.c_str());
        return rime->deploy_config_file(fileName.c_str(), versionKey.c_str());
    }

// Query
    string JRimeCore::getCommit() {
        LOGV("getCommit()");
        SessionId const s { getSession() };
        auto session { Service::instance().GetSession(s) };
        if (!session) {
            LOGE("Session(0x%lX) is dead", s);
            throw std::runtime_error("Session is dead");
        }
        #ifdef DEBUG_PRINT_PROTO
        LOGD("{ \"Commit\": \"%s\", }", session->commit_text().c_str());
        #endif // DEBUG_PRINT_PROTO
        return session->commit_text();
    }
    string JRimeCore::getPreedit() {
        LOGV("getPreedit()");
        SessionId const s { getSession() };
        auto session { Service::instance().GetSession(s) };
        if (!session) {
            LOGE("Session(0x%lX) is dead", s);
            throw std::runtime_error("Session is dead");
        }
        Context * const ctx { session->context() };
        if (!ctx) {
            LOGE("Session(0x%lX) has no context", s);
            throw std::runtime_error("Session has no context");
        }
        if (!ctx->IsComposing()) {
            LOGV("Session(0x%lX) is not composing", s);
            return "";
        }
        #ifdef DEBUG_PRINT_PROTO
        LOGD("{ \"Preedit\": \"%s\", }", ctx->GetPreedit().text.c_str());
        #endif // DEBUG_PRINT_PROTO

        return ctx->GetPreedit().text;
    }

    JRimeCore::JRimeCore(
        string const & sharedDataDir,
        string const & userDataDir,
        string const & appName
    ) : sharedDataDir { sharedDataDir }, userDataDir { userDataDir }, appName { appName } {
        LOGV(
            "Ctor(\n\tsharedDataDir = %s,\n\tuserDataDir = %s,\n\tappName = %s\n)",
            sharedDataDir.c_str(), userDataDir.c_str(), appName.c_str()
        );
        RIME_STRUCT(RimeTraits, traits);
        traits.shared_data_dir        = sharedDataDir.c_str();
        traits.user_data_dir          = userDataDir.c_str();
        traits.log_dir                = "";
        traits.app_name               = appName.c_str();
        traits.distribution_name      = "Rime for Android";
        traits.distribution_code_name = "Android";
        traits.distribution_version   = RIME_VERSION;
        rime->setup(&traits);
        rime->initialize(&traits);
        rime->set_notification_handler(nullptr, nullptr);
        LOGV("Ctor done");
    }
    JRimeCore::~JRimeCore() { LOGV("Dtor()"); }

// Private:
    RimeSessionId JRimeCore::getSession() {
        if (!session) {
            try {
                session = std::make_shared<SessionTracker>(rime);
            } catch (std::runtime_error const & e) {
                session = nullptr;
                LOGV("getSession() Failed: %s", e.what());
                return {};
            }
        }
        LOGV("getSession() = 0x%lX", session->id());
        return session->id();
    }

}
