//
// Created by kchen on 11/30/17.
//

#pragma once

#ifndef SPLUNK_HEC_CLIENT_CPP_JSON_EVENT_BATCH_H
#define SPLUNK_HEC_CLIENT_CPP_JSON_EVENT_BATCH_H

#include <string>

#include "event_batch.h"

namespace splunkhec {

class JsonEventBatch final: public EventBatch {
public:
    std::string rest_endpoint() const override;
    std::string content_type() const override;
    JsonEventBatch* create_from_this() const override;

private:
    static const std::string event_endpoint;
    static const std::string event_content_type;
};

} // namespace splunkhec

#endif //SPLUNK_HEC_CLIENT_CPP_JSON_EVENT_BATCH_H
