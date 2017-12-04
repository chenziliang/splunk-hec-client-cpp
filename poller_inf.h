//
// Created by kchen on 12/3/17.
//

#ifndef SPLUNK_HEC_CLIENT_CPP_POLLER_INF_H
#define SPLUNK_HEC_CLIENT_CPP_POLLER_INF_H

#include <string>
#include <exception>

#include "event_batch.h"
#include "hec_channel.h"

namespace splunkhec {

class PollerInf {
public:
    virtual ~PollerInf() {}
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void add(const HecChannel& channel, const EventBatch& batch, const std::string& response) = 0;
    virtual void fail(const HecChannel& channel, const EventBatch& batch, const std::exception& ex) = 0;
};

} // namespace splunkhec

#endif //SPLUNK_HEC_CLIENT_CPP_POLLER_INF_H
