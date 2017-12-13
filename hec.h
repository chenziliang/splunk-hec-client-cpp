//
// Created by kchen on 12/12/17.
//

#ifndef SPLUNK_HEC_CLIENT_CPP_HEC_H
#define SPLUNK_HEC_CLIENT_CPP_HEC_H

#include "hec_inf.h"
#include "poller_inf.h"
#include "load_balancer_inf.h"
#include "poller_callback_inf.h"
#include "config.h"

#include <memory>

namespace splunkhec {

class Hec final: public HecInf {
public:
    static HecInf* newHecWithAck(const Config& config, const std::shared_ptr<PollerCallbackInf>& callback);
    static HecInf* newHecWithoutAck(const Config& config, const std::shared_ptr<PollerCallbackInf>& callback);

public:
    Hec(const Config& config, const std::shared_ptr<PollerInf>& poller, const std::shared_ptr<LoadBalancerInf>& lb);
    ~Hec() {
        poller_->stop();
    }

    bool send(const std::shared_ptr<EventBatch>& batch) override;

private:
    std::shared_ptr<PollerInf> poller_;
    std::shared_ptr<LoadBalancerInf> loadbalancer_;
};

} // namespace splunkhec

#endif //SPLUNK_HEC_CLIENT_CPP_HEC_H
