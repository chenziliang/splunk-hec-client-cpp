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

void HttpResponsePoller::add(const HecChannel& channel, const shared_ptr<EventBatch>& batch, const string& response) {
    // response is json as below
    // {"text":"Success","code":0, "ackId":7}

    Document doc;
    doc.Parse(response.data(), response.size());
    auto pos{doc.FindMember("code")};
    if (pos == doc.MemberEnd() || pos->value != 0) {
        fail(channel, batch, HecException(response));
        return;
    }

    batch->commit();
    if (callback_) {
        callback_->on_event_committed(vector<shared_ptr<EventBatch>>{batch});
    }
}

void HttpResponsePoller::fail(const HecChannel& channel, const shared_ptr<EventBatch>& batch, const exception& ex) {
    batch->fail();
    if (callback_) {
        callback_->on_event_failed(vector<shared_ptr<EventBatch>>{batch}, ex);
    }
}

} // namespace splunkhec
