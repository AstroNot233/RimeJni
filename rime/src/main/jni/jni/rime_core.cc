#include "rime_core.hh"
#include "debug/logger.hh"

#include <librime/rime/service.h>
#include <librime/rime/context.h>
#include <librime/rime/schema.h>
#include "rime_session.hh"

namespace rime::jni {

    using std::string, std::vector;

// Lifecycle
    Bool JRimeCore::startup(Bool fullCheck) noexcept {
        LOGV("startup(fullCheck = %d)", fullCheck);
        rime->initialize(&traits);
        return rime->start_maintenance(fullCheck);
    }
    void JRimeCore::shutdown() noexcept {
        LOGV("shutdown()");
        return rime->finalize();
    }
    Bool JRimeCore::syncUserData() noexcept {
        LOGV("syncUserData()");
        return rime->sync_user_data();
    }

// IO behavior
    Bool JRimeCore::processKey(int keyCode, int mask) noexcept {
        LOGV("processKey(keyCode = %d, mask = %d)", keyCode, mask);
        RimeSessionId sId { getSessionId() };
        if (!sId)
            return False;
        return rime->process_key(sId, keyCode, mask);
    }
    Bool JRimeCore::simulateKeySequence(string const & sequence) noexcept {
        LOGV("simulateKeySequence(sequence = %s)", sequence.c_str());
        RimeSessionId sId { getSessionId() };
        if (!sId)
            return False;
        return rime->simulate_key_sequence(sId, sequence.c_str());
    }
    Bool JRimeCore::commitComposition() noexcept {
        LOGV("commitComposition()");
        RimeSessionId sId { getSessionId() };
        if (!sId)
            return False;
        return rime->commit_composition(sId);
    }
    void JRimeCore::clearComposition() noexcept {
        LOGV("clearComposition()");
        RimeSessionId sId { getSessionId() };
        if (!sId)
            return;
        return rime->clear_composition(sId);
    }

// Option
    void JRimeCore::setOption(string const & option, Bool value) noexcept {
        LOGV("setOption(option = %s, value = %d)", option.c_str(), value);
        RimeSessionId sId { getSessionId() };
        if (!sId)
            return;
        return rime->set_option(sId, option.c_str(), value);
    }
    Bool JRimeCore::getOption(string const & option) noexcept {
        LOGV("getOption(option = %s)", option.c_str());
        RimeSessionId sId { getSessionId() };
        if (!sId)
            return False;
        return rime->get_option(sId, option.c_str());
    }
    void JRimeCore::setProperty(string const & property, string const & value) noexcept {
        LOGV("setProperty(property = %s, value = %s)", property.c_str(), value.c_str());
        RimeSessionId sId { getSessionId() };
        if (!sId)
            return;
        return rime->set_property(sId, property.c_str(), value.c_str());
    }
    string JRimeCore::getProperty(string const & property) noexcept {
        LOGV("getProperty(property = %s)", property.c_str());
        RimeSessionId sId { getSessionId() };
        if (!sId)
            return "";
        char value[MAX_BUFFER_LENGTH] {};
        if (rime->get_property(sId, property.c_str(), value, MAX_BUFFER_LENGTH)) {
            return value;
        } else {
            return "";
        }
    }

// Schema
    Bool JRimeCore::deploySchema(string const & schemaFile) noexcept {
        LOGV("deploySchema(schemaFile = %s)", schemaFile.c_str());
        return rime->deploy_schema(schemaFile.c_str());
    }
    vector<JRimeSchema> JRimeCore::getSchemata() noexcept {
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
    JRimeSchema JRimeCore::getCurrentSchema() noexcept {
        LOGV("getCurrentSchema()");
        RimeSessionId sId { getSessionId() };
        if (!sId)
            return { "", "" };
        auto session { Service::instance().GetSession(sId) };
        Schema * const schema { session->schema() };
        return { schema->schema_id(), schema->schema_name() };
    }
    Bool JRimeCore::selectSchema(string const & schemaId) noexcept {
        LOGV("selectSchema(schemaId = %s)", schemaId.c_str());
        RimeSessionId sId { getSessionId() };
        if (!sId)
            return False;
        return rime->select_schema(sId, schemaId.c_str());
    }

// Candidate and page
    vector<JRimeCandidate> JRimeCore::getCandidates() noexcept {
        LOGV("getCandidates()");
        RimeSessionId sId { getSessionId() };
        if (!sId)
            return vector<JRimeCandidate>(0);
        RimeCandidateListIterator iter {};
        if (!rime->candidate_list_from_index(sId, &iter, 0))
            return vector<JRimeCandidate>(0);
        vector<JRimeCandidate> rimeCandidateList {};
        rimeCandidateList.reserve(MAX_CANDIDATE_COUNT);
        for (size_t i { 0U }; i < MAX_CANDIDATE_COUNT && rime->candidate_list_next(&iter); ++i)
            rimeCandidateList.emplace_back(JRimeCandidate {
                iter.candidate.text ?: "",
                iter.candidate.comment ?: ""
        });
        rime->candidate_list_end(&iter);
        return rimeCandidateList;
    }
    Bool JRimeCore::selectCandidate(int index) noexcept {
        LOGV("selectCandidate(index = %d)", index);
        RimeSessionId sId { getSessionId() };
        if (!sId)
            return False;
        return rime->select_candidate(sId, index);
    }
    Bool JRimeCore::deleteCandidate(int index) noexcept {
        LOGV("deleteCandidate(index = %d)", index);
        RimeSessionId sId { getSessionId() };
        if (!sId)
            return False;
        return rime->delete_candidate(sId, index);
    }
    Bool JRimeCore::highlightCandidate(int index) noexcept {
        LOGV("highlightCandidate(index = %d)", index);
        RimeSessionId sId { getSessionId() };
        if (!sId)
            return False;
        return rime->highlight_candidate(sId, index);
    }
    Bool JRimeCore::changePage(Bool backward) noexcept {
        LOGV("changePage(backward = %d)", backward);
        RimeSessionId sId { getSessionId() };
        if (!sId)
            return False;
        return rime->change_page(sId, backward);
    }

// Config
    Bool JRimeCore::deployConfigFile(string const & fileName, string const & versionKey) noexcept {
        LOGV("deployConfigFile(fileName = %s, versionKey = %s)", fileName.c_str(), versionKey.c_str());
        return rime->deploy_config_file(fileName.c_str(), versionKey.c_str());
    }

// Query
    int JRimeCore::getStatus() noexcept {
        LOGV("getStatus()");
        RimeSessionId sId { getSessionId() };
        if (!sId)
            return 0;
        auto session { Service::instance().GetSession(sId) };
        Context * const context { session->context() };
        return
            ((Service::instance().disabled())        << 0) |
            ((context->IsComposing())                << 1) |
            ((context->get_option("ascii_mode"))     << 2) |
            ((context->get_option("full_shape"))     << 3) |
            ((context->get_option("simplification")) << 4) |
            ((context->get_option("traditional"))    << 5) |
            ((context->get_option("ascii_punct"))    << 6) ;
    }
    string JRimeCore::getCommit() noexcept {
        LOGV("getCommit()");
        RimeSessionId sId { getSessionId() };
        if (!sId)
            return "";
        auto session { Service::instance().GetSession(sId) };
        string commit { session->commit_text() };
        session->ResetCommitText();
        return commit;
    }
    JRimePreedit JRimeCore::getPreedit() noexcept {
        LOGV("getPreedit()");
        RimeSessionId sId { getSessionId() };
        if (!sId)
            return { "", 0, 0, 0 };
        auto session { Service::instance().GetSession(sId) };
        Context * const context { session->context() };
        auto const preedit { context->GetPreedit() };

        char const * const bytes { preedit.text.data() };
        auto const size { preedit.text.size() };
        unsigned long caretPos { 0 }, selStart { 0 }, selEnd { 0 };
        for (unsigned long i { 0 }, cnt { 1 }; i < size; ++cnt) {
            auto const c { static_cast<unsigned char>(bytes[i]) };
            if (c < 0x80) i += 1;
            else if ((c & 0xE0) == 0xC0) i += 2;
            else if ((c & 0xF0) == 0xE0) i += 3;
            else if ((c & 0xF8) == 0xF0) i += 4;
            else i += 1;
            if (i == preedit.caret_pos)
                caretPos = cnt;
            if (i == preedit.sel_start)
                selStart = cnt;
            if (i == preedit.sel_end)
                selEnd = cnt;
        }
        return { preedit.text, caretPos, selStart, selEnd };
    }

    JRimeCore::JRimeCore(
        string const & sharedDataDir,
        string const & userDataDir,
        string const & appName
    ) noexcept : sharedDataDir { sharedDataDir }, userDataDir { userDataDir }, appName { appName },
        traits { 
            sizeof(rime_traits_t) - sizeof(rime_traits_t::data_size),
            this->sharedDataDir.c_str(), this->userDataDir.c_str(),
            distribution_name, distribution_code_name, distribution_version,
            this->appName.c_str(),
            nullptr,    //           modules = delete
            0,          //     min_log_level = default -> INFO
            "",         //           log_dir = delete
            nullptr,    // prebuilt_data_dir = default -> ${shared_data_dir}/build
            nullptr     //       staging_dir = default -> ${user_data_dir}/build
        }
    {
        LOGI(
            "Ctor(\n\tsharedDataDir = %s,\n\tuserDataDir = %s,\n\tappName = %s\n)",
            sharedDataDir.c_str(), userDataDir.c_str(), appName.c_str()
        );
        rime->setup(&traits);
        rime->set_notification_handler(nullptr, nullptr);
        LOGI("Ctor done");
    }
    JRimeCore::~JRimeCore() noexcept {
        LOGI("Dtor()");
        LOGI("Dtor done");
    }

// Private:
    RimeSessionId JRimeCore::getSessionId() noexcept {
        static std::shared_ptr<SessionTracker> session { nullptr };
        if (session && !session->operator bool())
            session = nullptr;
        if (!session) {
            try {
                session = std::make_shared<SessionTracker>(rime);
            } catch (std::runtime_error const & e) {
                session = nullptr;
                LOGE("getSessionId() Failed: %s", e.what());
                return 0UL;
            }
        }
        LOGV("getSessionId() -> 0x%lX", session->id());
        return session->id();
    }

}
