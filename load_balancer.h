//
// Created by kchen on 12/12/17.
//

#pragma once

#ifndef SPLUNK_HEC_CLIENT_CPP_LOAD_BALANCER_H
#define SPLUNK_HEC_CLIENT_CPP_LOAD_BALANCER_H

#include "load_balancer_inf.h"
#include "indexer_inf.h"

#include <memory>
#include <vector>

namespace splunkhec {

class LoadBalancer final: public LoadBalancerInf {
public:
    void add(const std::shared_ptr<IndexerInf>& indexer) override;
    void remove(const std::shared_ptr<IndexerInf>& indexer) override;
    bool send(const std::shared_ptr<EventBatch>& batch) override ;
    std::size_t size() const override;

private:
    std::vector<std::shared_ptr<IndexerInf>> indexers_;
    int index_ = 0;
};

} // namespace splunkhec

#endif //SPLUNK_HEC_CLIENT_CPP_LOAD_BALANCER_H
