//
// Created by kchen on 12/13/17.
//

#ifndef SPLUNK_HEC_CLIENT_CPP_HEC_INF_H
#define SPLUNK_HEC_CLIENT_CPP_HEC_INF_H

#include "event_batch.h"

#include <memory>

namespace splunkhec {

class HecInf {
public:
    virtual ~HecInf() {}
    virtual bool send(const std::shared_ptr<EventBatch>& batch) = 0;
    virtual void stop() {}
};

} // namespace splunkhec

#endif //SPLUNK_HEC_CLIENT_CPP_HEC_INF_H
