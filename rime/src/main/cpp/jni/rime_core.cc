#include "rime_jni.hpp"
#include "fbjni_alias.hpp"

namespace rime {

// class JRimeCore : public facebook::jni::HybridClass<JRimeCore>

// Factory method
    local_ref<JRimeCore::javaobject> JRimeCore::create(alias_ref<JClass> /*self*/, jstring sharedDataDir, jstring userDataDir, jstring appName, jobject notification) {
        return newObjectCxxArgs(sharedDataDir, userDataDir, appName, notification);
    }

// Lifecycle
    jboolean JRimeCore::startup(jboolean fullCheck) {
        rime->setup(traits.get());
        rime->initialize(traits.get());
        rime->set_notification_handler(notificationHandler, const_cast<JRimeCore *>(this));
        return rime->start_maintenance(fullCheck);
    }
    void JRimeCore::shutdown() {
        session = 0u;
        return rime->finalize();
    }
    jboolean JRimeCore::syncUserData() {
        return rime->sync_user_data();
    }

// IO behavior
    jboolean JRimeCore::processKey(jint keyCode, jint mask) {
        return rime->process_key(getSession(), keyCode, mask);
    }
    jboolean JRimeCore::simulateKeySequence(jstring sequence) {
        cstr_from_jstring s{sequence};
        return rime->simulate_key_sequence(getSession(), s);
    }
    jboolean JRimeCore::commitComposition() {
        return rime->commit_composition(getSession());
    }
    void JRimeCore::clearComposition() {
        return rime->clear_composition(getSession());
    }

// Option
    void JRimeCore::setOption(jstring option, jboolean value) {
        cstr_from_jstring o{option};
        return rime->set_option(getSession(), o, value);
    }
    jboolean JRimeCore::getOption(jstring option) {
        cstr_from_jstring o{option};
        return rime->get_option(getSession(), o);
    }
    void JRimeCore::setProperty(jstring property, jstring value) {
        auto const p{cstr_from_jstring(property)};
        auto const j{cstr_from_jstring(value)};
        return rime->set_property(getSession(), p, j);
    }
    jstring JRimeCore::getProperty(jstring property) {
        auto const p{cstr_from_jstring(property)};
        char value[MAX_BUFFER_LENGTH];
        if (rime->get_property(getSession(), p, value, MAX_BUFFER_LENGTH)) {
            return jstring_from_cstr(value);
        } else {
            return jstring_from_cstr();
        }
    }

// Schema
    jboolean JRimeCore::deploySchema(jstring schemaFile) {
        cstr_from_jstring sf{schemaFile};
        return rime->deploy_schema(sf);
    }
    jobject JRimeCore::getSchemaList() {
        RimeSchemaList list{};
        if (rime->get_schema_list(&list)) {
            auto rimeSchemaList{jarray<jobject>::newArray(list.size)};
            for (int i{}; i < list.size; ++i) {
                auto obj{
                        JRimeSchemaInfo::create(
                                jstring_from_cstr(list.list[i].schema_id),
                                jstring_from_cstr(list.list[i].name)
                        )
                };
                rimeSchemaList->setElement(i, obj.get());
            }
            rime->free_schema_list(&list);
            return rimeSchemaList.release();
        } else {
            return jarray<jobject>::newArray(0).release();
        }
    }
    jstring JRimeCore::getCurrentSchemaId() {
        char currentSchemaId[MAX_BUFFER_LENGTH];
        if (rime->get_current_schema(getSession(), currentSchemaId, MAX_BUFFER_LENGTH)) {
            return jstring_from_cstr(currentSchemaId);
        } else {
            return jstring_from_cstr();
        }
    }
    jboolean JRimeCore::selectSchema(jstring schemaId) {
        cstr_from_jstring si{schemaId};
        return rime->select_schema(getSession(), si);
    }

// Candidate and page
    jboolean JRimeCore::selectCandidate(jint index) {
        return rime->select_candidate(getSession(), index);
    }
    jboolean JRimeCore::deleteCandidate(jint index) {
        return rime->delete_candidate(getSession(), index);
    }
    jboolean JRimeCore::highlightCandidate(jint index) {
        return rime->highlight_candidate(getSession(), index);
    }
    jboolean JRimeCore::changePage(jboolean backward) {
        return rime->change_page(getSession(), backward);
    }

// Config
    jboolean JRimeCore::deployConfigFile(jstring fileName, jstring versionKey) {
        cstr_from_jstring fn{fileName}, vk{versionKey};
        return rime->deploy_config_file(fn, vk);
    }

// Register native method
    void JRimeCore::RegisterNatives() {
        registerHybrid(
            {
                // Factory method
                makeNativeMethod("create", JRimeCore::create),
                // Lifecycle
                makeNativeMethod("startup", JRimeCore::startup),
                makeNativeMethod("shutdown", JRimeCore::shutdown),
                makeNativeMethod("syncUserData", JRimeCore::syncUserData),
                // IO behavior
                makeNativeMethod("processKey", JRimeCore::processKey),
                makeNativeMethod("simulateKeySequence", JRimeCore::simulateKeySequence),
                makeNativeMethod("commitComposition", JRimeCore::commitComposition),
                makeNativeMethod("clearComposition", JRimeCore::clearComposition),
                // Option
                makeNativeMethod("setOption", JRimeCore::setOption),
                makeNativeMethod("getOption", JRimeCore::getOption),
                makeNativeMethod("setProperty", JRimeCore::setProperty),
                makeNativeMethod("getProperty", JRimeCore::getProperty),
                // Schema
                makeNativeMethod("deploySchema", JRimeCore::deploySchema),
                makeNativeMethod("getSchemaList", JRimeCore::getSchemaList),
                makeNativeMethod("getCurrentSchemaId", JRimeCore::getCurrentSchemaId),
                makeNativeMethod("selectSchema", JRimeCore::selectSchema),
                // Candidate and page
                makeNativeMethod("selectCandidate", JRimeCore::selectCandidate),
                makeNativeMethod("deleteCandidate", JRimeCore::deleteCandidate),
                makeNativeMethod("highlightCandidate", JRimeCore::highlightCandidate),
                makeNativeMethod("changePage", JRimeCore::changePage),
                // Config
                makeNativeMethod("deployConfigFile", JRimeCore::deployConfigFile)
            }
        );
    }

// private:

    JRimeCore::JRimeCore(jstring sharedDataDir, jstring userDataDir, jstring appName, jobject notification) :
    notification { make_global(notification) },
    traits {
        std::make_shared<RimeTraitsAndroid> (
            cstr_from_jstring(sharedDataDir).copy(),
            cstr_from_jstring(userDataDir).copy(),
            rime->get_version(),
            cstr_from_jstring(appName).copy()
        )
    } {}

    RimeSessionId JRimeCore::getSession(bool newSession) {
        if (newSession && session)
            rime->destroy_session(session);
        if (!session)
            session = rime->create_session();
        return session;
    }

    void JRimeCore::notificationHandler(void * context_object, RimeSessionId session_id, char const * message_type, char const * message_value) {
        auto const notification = static_cast<JRimeCore *>(context_object)->notification;
        return // return notification.handle(message_type, message_value);
            (notification->getClass()->getMethod<void(jstring, jstring)>("create"))(
                    notification.get(),
                    jstring_from_cstr(message_type),
                    jstring_from_cstr(message_value)
            );
    }

}
