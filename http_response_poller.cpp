//
// Created by kchen on 12/3/17.
//

#include "http_response_poller.h"
#include "hec_exception.h"

#include "rapidjson/include/rapidjson/document.h"

using namespace std;
using namespace rapidjson;

namespace splunkhec {

HttpResponsePoller::HttpResponsePoller(const shared_ptr<PollerCallbackInf>& callback)
        : callback_(callback) {
}

void HttpResponsePoller::start() {}

void HttpResponsePoller::stop() {}

void HttpResponsePoller::add(const shared_ptr<IndexerInf>& indexer, const shared_ptr<EventBatch>& batch, const string& response) {
    // {"text":"Success","code":0, "ackId":7}

    Document doc;
    doc.Parse(response.data(), response.size());
    if (doc.HasParseError()) {
        return fail(indexer, batch, HecException(response, -1));
    }

    auto it{doc.FindMember("code")};
    if (it == doc.MemberEnd() || !it->value.IsInt()) {
        return fail(indexer, batch, HecException(response, -1));
    }

    if (it->value.GetInt() != 0) {
        return fail(indexer, batch, HecException(response, it->value.GetInt()));
    }

    batch->commit();
    if (callback_) {
        callback_->on_event_committed(vector<shared_ptr<EventBatch>>{batch});
    }
}

void HttpResponsePoller::fail(const shared_ptr<IndexerInf>& indexer, const shared_ptr<EventBatch>& batch, const exception& ex) {
    batch->fail();
    if (callback_) {
        callback_->on_event_failed(vector<shared_ptr<EventBatch>>{batch}, ex);
    }
}

} // namespace splunkhec
