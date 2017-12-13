//
// Created by kchen on 12/13/17.
//

#include "concurrent_hec.h"
#include "hec.h"

#include <functional>
#include <algorithm>

using namespace std;

namespace splunkhec {

ConcurrentHec::ConcurrentHec(size_t threads, bool use_ack, Config &config, const shared_ptr<PollerCallbackInf>& callback)
        : workers_{threads}, batches_{2 * threads}, stopped_{false} {
    new_hecs(threads, use_ack, config, callback);
    for (int i = 0; i < threads; ++i) {
        workers_.submit(bind(&ConcurrentHec::do_send, this, i));
    }
}

bool ConcurrentHec::send(const shared_ptr<EventBatch>& batch) {
    return batches_.put(batch, chrono::seconds{1});
}

void ConcurrentHec::stop() {
    stopped_ = true;
}

void ConcurrentHec::new_hecs(size_t num, bool use_ack, const Config& config, const shared_ptr<PollerCallbackInf>& callback) {
    Config c{config};
    c.total_channels_ = max(int(config.total_channels_ / num), 1);
    for (int i = 0; i < num; ++i) {
        HecInf* hec;
        if (use_ack) {
            hec = Hec::newHecWithAck(c, callback);
        } else {
            hec = Hec::newHecWithoutAck(c, callback);
        }
        hecs_.emplace_back(hec);
    }
}

void ConcurrentHec::do_send(int idx) {
    auto& hec = hecs_[idx];
    while (!stopped_.load()) {
        shared_ptr<EventBatch> batch;
        bool ok = batches_.get(batch, chrono::seconds{1});
        if (ok) {
            hec->send(batch);
        }
    }
}

} // namespace splunkhec
