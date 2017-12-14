//
// Created by kchen on 12/3/17.
//

#include "http_response_poller.h"
#include "hec_exception.h"
#include "hec_response_parser.h"

using namespace std;
using namespace rapidjson;

namespace splunkhec {

HttpResponsePoller::HttpResponsePoller(const shared_ptr<PollerCallbackInf>& callback)
        : callback_(callback) {
}

void HttpResponsePoller::start() {}

void HttpResponsePoller::stop() {}

void HttpResponsePoller::add(const shared_ptr<IndexerInf>& indexer, const shared_ptr<EventBatch>& batch, const string& response) {
    int code = parse_response(response);
    if (code != 0) {
        return fail(indexer, batch, HecException(response, code));
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
