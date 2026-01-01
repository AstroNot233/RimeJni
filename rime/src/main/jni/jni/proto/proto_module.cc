#include <librime/rime/component.h>
#include <librime/rime/composition.h>
#include <librime/rime/context.h>
#include <librime/rime/menu.h>
#include <librime/rime/schema.h>
#include <librime/rime/service.h>
#include <librime/rime_api.h>
#include "rime_proto.capnp.h"
#include "rime_proto_api.h"

using namespace rime;

void rime_commit_proto(RimeSessionId session_id, RIME_PROTO_BUILDER* commit_builder) {
    an<Session> session(Service::instance().GetSession(session_id));
    if (!session)
        return;
    const string& commit_text(session->commit_text());
    if (!commit_text.empty()) {
        auto* commit = (rime::proto::Commit::Builder*)commit_builder;
        commit->setText(commit_text);
        session->ResetCommitText();
    }
}

void rime_context_proto(RimeSessionId session_id, RIME_PROTO_BUILDER* context_builder) {
    an<Session> session = Service::instance().GetSession(session_id);
    if (!session)
        return;
    Context *ctx = session->context();
    if (!ctx)
        return;
    auto* context = (rime::proto::Context::Builder*)context_builder;
    context->setInput(ctx->input());
    context->setCaret(ctx->caret_pos());
    if (ctx->IsComposing()) {
        auto composition = context->getComposition();
        Preedit preedit = ctx->GetPreedit();
        composition.setLength(preedit.text.length());
        composition.setPreedit(preedit.text);
        composition.setCursor(preedit.caret_pos);
        composition.setSelHead(preedit.sel_start);
        composition.setSelTail(preedit.sel_end);
        string commit_text = ctx->GetCommitText();
        if (!commit_text.empty()) {
            composition.setPreview(commit_text);
        }
    }
    if (ctx->HasMenu()) {
        auto menu = context->getMenu();
        Segment &seg = ctx->composition().back();
        Schema *schema = session->schema();
        int page_size = schema ? schema->page_size() : 5;
        int selected_index = seg.selected_index;
        int page_number = selected_index / page_size;
        the<Page> page(seg.menu->CreatePage(page_size, page_number));
        if (page) {
            menu.setPageSize(page_size);
            menu.setPageNum(page_number);
            menu.setLastPage(page->is_last_page);
            menu.setHighlight(selected_index % page_size);
            vector<string> labels;
            if (schema) {
                const string& select_keys = schema->select_keys();
                if (!select_keys.empty()) {
                    menu.setSelKeys(select_keys);
                }
                Config* config = schema->config();
                auto src_labels = config->GetList("menu/alternative_select_labels");
                if (src_labels && (size_t)page_size <= src_labels->size()) {
                    auto dest_labels = menu.initSelLabels(page_size);
                    for (size_t i = 0; i < (size_t)page_size; ++i) {
                        if (an<ConfigValue> value = src_labels->GetValueAt(i)) {
                            dest_labels.set(i, value->str());
                            labels.push_back(value->str());
                        }
                    }
                } else if (!select_keys.empty()) {
                    for (const char key : select_keys) {
                        labels.push_back(string(1, key));
                        if (labels.size() >= page_size)
                            break;
                    }
                }
            }
            int num_candidates = page->candidates.size();
            auto dest_candidates = menu.initCandidates(num_candidates);
            auto dest = dest_candidates.begin();
            int index = 0;
            for (const an<Candidate> &src : page->candidates) {
                dest->setText(src->text());
                string comment = src->comment();
                if (!comment.empty()) {
                    dest->setComment(comment);
                }
                string label = index < labels.size()
                    ? labels[index]
                    : std::to_string(index + 1);
                dest->setLabel(label);
                ++dest;
            }
        }
    }
}

void rime_status_proto(RimeSessionId session_id, RIME_PROTO_BUILDER* status_builder) {
    an<Session> session(Service::instance().GetSession(session_id));
    if (!session)
        return;
    Schema *schema = session->schema();
    Context *ctx = session->context();
    if (!schema || !ctx)
        return;
    auto* status = (rime::proto::Status::Builder*)status_builder;
    status->setSchemaId(schema->schema_id());
    status->setSchemaName(schema->schema_name());
    status->setDisabled(Service::instance().disabled());
    status->setComposing(ctx->IsComposing());
    status->setAsciiMode(ctx->get_option("ascii_mode"));
    status->setFullShape(ctx->get_option("full_shape"));
    status->setSimplified(ctx->get_option("simplification"));
    status->setTraditional(ctx->get_option("traditional"));
    status->setAsciiPunct(ctx->get_option("ascii_punct"));
}

RimeCustomApi* rime_proto_get_api() {
    static RimeProtoApi s_api ={0};
    if (!s_api.data_size) {
        RIME_STRUCT_INIT(RimeProtoApi, s_api);
        s_api.commit_proto = &rime_commit_proto;
        s_api.context_proto = &rime_context_proto;
        s_api.status_proto = &rime_status_proto;
    }
    return (RimeCustomApi*)&s_api;
}
