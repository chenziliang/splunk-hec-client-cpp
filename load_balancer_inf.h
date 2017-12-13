//
// Created by kchen on 12/12/17.
//

#ifndef SPLUNK_HEC_CLIENT_CPP_LOAD_BALANCER_INF_H
#define SPLUNK_HEC_CLIENT_CPP_LOAD_BALANCER_INF_H

#include "event_batch.h"
#include "indexer_inf.h"

#include <memory>

namespace splunkhec {

class LoadBalancerInf {
public:
    virtual ~LoadBalancerInf() {}
    virtual void add(const std::shared_ptr<IndexerInf>& indexer) = 0;
    virtual void remove(const std::shared_ptr<IndexerInf>& indexer) = 0;
    virtual bool send(const std::shared_ptr<EventBatch>& batch) = 0;
    virtual std::size_t size() const = 0;
};

} // namespace splunkhec

#endif //SPLUNK_HEC_CLIENT_CPP_LOAD_BALANCER_INF_H
