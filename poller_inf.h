//
// Created by kchen on 12/3/17.
//

#ifndef SPLUNK_HEC_CLIENT_CPP_POLLER_INF_H
#define SPLUNK_HEC_CLIENT_CPP_POLLER_INF_H

#include "event_batch.h"
#include "indexer_inf.h"

#include <exception>
#include <memory>
#include <string>

namespace splunkhec {

class PollerInf {
public:
    virtual ~PollerInf() {}
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void add(const std::shared_ptr<IndexerInf>& indexer, const std::shared_ptr<EventBatch>& batch, const std::string& response) = 0;
    virtual void fail(const std::shared_ptr<IndexerInf>& indexer, const std::shared_ptr<EventBatch>& batch, const std::exception& ex) = 0;
};

} // namespace splunkhec

#endif //SPLUNK_HEC_CLIENT_CPP_POLLER_INF_H
