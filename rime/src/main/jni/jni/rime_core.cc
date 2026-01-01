#include "rime_jni.hpp"

#include "rime_proto.capnp.h"
#include <capnp/message.h>
#include <capnp/compat/json.h>
#include <kj/string.h>

namespace rime::jni {
// class JRimeCore

// Lifecycle
    Bool JRimeCore::startup(Bool fullCheck) {
        LOGI("startup(fullCheck = %d)", fullCheck);
        return rime->start_maintenance(fullCheck);
    }
    void JRimeCore::shutdown() {
        LOGI("shutdown()");
        session = 0u;
        return rime->finalize();
    }
    Bool JRimeCore::syncUserData() {
        LOGI("syncUserData()");
        return rime->sync_user_data();
    }

// IO behavior
    Bool JRimeCore::processKey(int keyCode, int mask) {
        LOGI("processKey(keyCode = %d, mask = %d)", keyCode, mask);
        return rime->process_key(getSession(), keyCode, mask);
    }
    Bool JRimeCore::simulateKeySequence(std::string const & sequence) {
        LOGI("simulateKeySequence(sequence = %s)", sequence.c_str());
        return rime->simulate_key_sequence(getSession(), sequence.c_str());
    }
    Bool JRimeCore::commitComposition() {
        LOGI("commitComposition()");
        return rime->commit_composition(getSession());
    }
    void JRimeCore::clearComposition() {
        LOGI("clearComposition()");
        return rime->clear_composition(getSession());
    }

// Option
    void JRimeCore::setOption(std::string const & option, Bool value) {
        LOGI("setOption(option = %s, value = %d)", option.c_str(), value);
        return rime->set_option(getSession(), option.c_str(), value);
    }
    Bool JRimeCore::getOption(std::string const & option) {
        LOGI("getOption(option = %s)", option.c_str());
        return rime->get_option(getSession(), option.c_str());
    }
    void JRimeCore::setProperty(std::string const & property, std::string const & value) {
        LOGI("setProperty(property = %s, value = %s)", property.c_str(), value.c_str());
        return rime->set_property(getSession(), property.c_str(), value.c_str());
    }
    std::string JRimeCore::getProperty(std::string const & property) {
        LOGI("getProperty(property = %s)", property.c_str());
        char value[MAX_BUFFER_LENGTH];
        if (rime->get_property(getSession(), property.c_str(), value, MAX_BUFFER_LENGTH)) {
            return value;
        } else {
            return "";
        }
    }

// Schema
    Bool JRimeCore::deploySchema(std::string const & schemaFile) {
        LOGI("deploySchema(schemaFile = %s)", schemaFile.c_str());
        return rime->deploy_schema(schemaFile.c_str());
    }
    std::vector<JRimeSchemaInfo> JRimeCore::getSchemaList() {
        LOGI("getSchemaList()");
        RimeSchemaList list {};
        if (rime->get_schema_list(&list)) {
            std::vector<JRimeSchemaInfo> rimeSchemaList {};
            rimeSchemaList.reserve(list.size);
            for (size_t i { 0 }; i < list.size; ++i) {
                rimeSchemaList.emplace_back(JRimeSchemaInfo {
                    std::string(list.list[i].schema_id),
                    std::string(list.list[i].name)
                });
            }
            rime->free_schema_list(&list);
            return rimeSchemaList;
        } else {
            return std::vector<JRimeSchemaInfo>(0);
        }
    }
    std::string JRimeCore::getCurrentSchemaId() {
        LOGI("getCurrentSchemaId()");
        char currentSchemaId[MAX_BUFFER_LENGTH];
        if (rime->get_current_schema(getSession(), currentSchemaId, MAX_BUFFER_LENGTH)) {
            return currentSchemaId;
        } else {
            return "";
        }
    }
    Bool JRimeCore::selectSchema(std::string const & schemaId) {
        LOGI("selectSchema(schemaId = %s)", schemaId.c_str());
        return rime->select_schema(getSession(), schemaId.c_str());
    }

// Candidate and page
    Bool JRimeCore::selectCandidate(int index) {
        LOGI("selectCandidate(index = %d)", index);
        return rime->select_candidate(getSession(), index);
    }
    Bool JRimeCore::deleteCandidate(int index) {
        LOGI("deleteCandidate(index = %d)", index);
        return rime->delete_candidate(getSession(), index);
    }
    Bool JRimeCore::highlightCandidate(int index) {
        LOGI("highlightCandidate(index = %d)", index);
        return rime->highlight_candidate(getSession(), index);
    }
    Bool JRimeCore::changePage(Bool backward) {
        LOGI("changePage(backward = %d)", backward);
        return rime->change_page(getSession(), backward);
    }

// Config
    Bool JRimeCore::deployConfigFile(std::string const & fileName, std::string const & versionKey) {
        LOGI("deployConfigFile(fileName = %s, versionKey = %s)", fileName.c_str(), versionKey.c_str());
        return rime->deploy_config_file(fileName.c_str(), versionKey.c_str());
    }

// Proto
    std::string JRimeCore::getCommitProtoJson() {
        LOGI("getCommitProtoJson()");
        capnp::MallocMessageBuilder message;
        proto::Commit::Builder commit { message.initRoot<proto::Commit>() };
        proto->commit_proto(getSession(), &commit);
        capnp::JsonCodec json;
        kj::String jsonString { json.encode(commit.asReader()) };
        LOGI("CommitProto: %s", jsonString.cStr());
        return jsonString.cStr();
    }
    std::string JRimeCore::getContextProtoJson() {
        LOGI("getContextProtoJson()");
        capnp::MallocMessageBuilder message;
        proto::Context::Builder context { message.initRoot<proto::Context>() };
        proto->context_proto(getSession(), &context);
        capnp::JsonCodec json;
        kj::String jsonString { json.encode(context.asReader()) };
        LOGI("ContextProto: %s", jsonString.cStr());
        return jsonString.cStr();
    }
    std::string JRimeCore::getStatusProtoJson() {
        LOGI("getStatusProtoJson()");
        capnp::MallocMessageBuilder message;
        proto::Status::Builder status { message.initRoot<proto::Status>() };
        proto->status_proto(getSession(), &status);
        capnp::JsonCodec json;
        kj::String jsonString { json.encode(status.asReader()) };
        LOGI("StatusProto: %s", jsonString.cStr());
        return jsonString.cStr();
    }

    JRimeCore::JRimeCore(
        std::string const & sharedDataDir,
        std::string const & userDataDir,
        std::string const & appName
    ) : sharedDataDir { sharedDataDir }, userDataDir { userDataDir }, appName { appName } {
        LOGI(
            "Ctor(\n\tsharedDataDir = %s,\n\tuserDataDir = %s,\n\tappName = %s\n)",
            sharedDataDir.c_str(), userDataDir.c_str(), appName.c_str()
        );
        RIME_STRUCT(RimeTraits, traits);
        traits.shared_data_dir        = sharedDataDir.c_str();
        traits.user_data_dir          = userDataDir.c_str();
        traits.log_dir                = "";
        traits.app_name               = appName.c_str();
        traits.distribution_name      = "Rime for Android";
        traits.distribution_code_name = "astronot233";
        traits.distribution_version   = RIME_VERSION;
        rime->setup(&traits);
        rime->initialize(&traits);
        rime->set_notification_handler(nullptr, nullptr);
        LOGI("Ctor done");
    }
    JRimeCore::~JRimeCore() { LOGI("Dtor()"); }

// Private:
    RimeSessionId JRimeCore::getSession(bool newSession) {
        if (newSession && session)
            rime->destroy_session(session);
        if (!session)
            session = rime->create_session();
        LOGI("getSession(newSession = %d) = 0x%lX", newSession, session);
        return session;
    }

}
