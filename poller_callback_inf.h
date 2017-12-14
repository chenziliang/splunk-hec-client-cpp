//
// Created by kchen on 12/3/17.
//

#pragma once

#ifndef SPLUNK_HEC_CLIENT_CPP_POLLER_CALLBACK_INF_H
#define SPLUNK_HEC_CLIENT_CPP_POLLER_CALLBACK_INF_H

#include "event_batch.h"

#include <vector>
#include <memory>

namespace splunkhec {

class PollerCallbackInf {
public:
    ~PollerCallbackInf() {}
    virtual void on_event_failed(const std::vector<std::shared_ptr<EventBatch>>& failure, const std::exception& ex) = 0;
    virtual void on_event_committed(const std::vector<std::shared_ptr<EventBatch>>& committed) = 0;
};

} // namespace splunkhec

#endif //SPLUNK_HEC_CLIENT_CPP_POLLER_CALLBACK_INF_H
