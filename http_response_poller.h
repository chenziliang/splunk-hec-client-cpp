//
// Created by kchen on 12/3/17.
//

#ifndef SPLUNK_HEC_CLIENT_CPP_HTTP_RESPONSE_POLLER_H
#define SPLUNK_HEC_CLIENT_CPP_HTTP_RESPONSE_POLLER_H

#include "poller_inf.h"
#include "poller_callback_inf.h"
#include "indexer_inf.h"
#include "event_batch.h"

#include <memory>
#include <exception>
#include <string>

namespace splunkhec {

class HttpResponsePoller: public PollerInf {
public:
    explicit HttpResponsePoller(const std::shared_ptr<PollerCallbackInf>& callback);

    void start() override;
    void stop() override;
    void add(const std::shared_ptr<IndexerInf>& indexer, const std::shared_ptr<EventBatch>& batch, const std::string& response) override;
    void fail(const std::shared_ptr<IndexerInf>& indexer, const std::shared_ptr<EventBatch>& batch, const std::exception& ex) override;

private:
    std::shared_ptr<PollerCallbackInf> callback_;
};

} // namespace splunkhec

#endif //SPLUNK_HEC_CLIENT_CPP_HTTP_RESPONSE_POLLER_H
