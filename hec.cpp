//
// Created by kchen on 12/12/17.
//

#include "hec.h"
#include "load_balancer.h"
#include "http_response_poller.h"
#include "http_client_factory.h"
#include "ack_poller.h"
#include "indexer.h"

using namespace std;

namespace splunkhec {

HecInf* Hec::newHecWithAck(const Config& config, const shared_ptr<PollerCallbackInf>& callback) {
    auto poller = make_shared<AckPoller>(config.ack_poll_threads_, callback);
    auto lb = make_shared<LoadBalancer>();
    return new Hec(config, poller, lb);
}

HecInf* Hec::newHecWithoutAck(const Config& config, const shared_ptr<PollerCallbackInf>& callback) {
    auto poller = make_shared<HttpResponsePoller>(callback);
    auto lb = make_shared<LoadBalancer>();
    return new Hec(config, poller, lb);
}

Hec::Hec(const Config& config, const shared_ptr<PollerInf>& poller, const shared_ptr<LoadBalancerInf>& lb)
        : poller_(poller), loadbalancer_(lb) {
    HttpClientFactory factory;
    factory.set_validate_certificates(config.http_validate_certs_)
            .set_chunksize(config.http_buffer_size_)
            .set_timeout(config.http_timeout_);

    for (int i = 0; i < config.total_channels_; ++i) {
        for (auto& uri: config.uris_) {
            auto indexer = make_shared<Indexer>(uri, config.token_, poller, factory);
            loadbalancer_->add(indexer);
        }
    }

    poller_->start();
}

bool Hec::send(const shared_ptr<EventBatch>& batch) {
    if (batch->empty()) {
        return false;
    }

    return loadbalancer_->send(batch);
}

}

// namespace splunkhec
