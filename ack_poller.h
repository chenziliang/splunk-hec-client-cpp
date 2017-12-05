//
// Created by kchen on 12/3/17.
//

#ifndef SPLUNK_HEC_CLIENT_CPP_ACK_POLLER_H
#define SPLUNK_HEC_CLIENT_CPP_ACK_POLLER_H

#include "poller_inf.h"

#include <memory>
#include <string>
#include <exception>
#include <unordered_map>
#include <chrono>
#include <atomic>

namespace splunkhec {

class HecChannel;
class EventBatch;

class AckPoller final: public PollerInf {
public:
    void start() override;
    void stop() override;
    void add(const HecChannel& channel, const std::shared_ptr<EventBatch>& batch, const std::string& response) override;
    void fail(const HecChannel& channel, const std::shared_ptr<EventBatch>& batch, const std::exception& ex) override;

private:
    static const std::string ack_endpoint_;

private:
    struct OutstandingBatches {
    public:
        OutstandingBatches(): lock_(new std::mutex()) {
        }

        OutstandingBatches(OutstandingBatches&& lhs): lock_(move(lhs.lock_)), batches_(move(lhs.batches_)) {
        }

        OutstandingBatches& operator=(OutstandingBatches&& lhs) {
            lock_ = move(lhs.lock_);
            batches_ = move(lhs.batches_);
            return *this;
        }

    private:
        std::unique_ptr<std::mutex> lock_;
        std::unordered_map<int64_t, std::shared_ptr<EventBatch>> batches_;
    };

private:
    std::mutex lock_;
    std::unordered_map<std::string, std::unique_ptr<OutstandingBatches>> outstanding_batches_;
    std::shared_ptr<PollerCallbackInf> poller_callback_;
    std::chrono::seconds event_batch_timeout_ = std::chrono::seconds(120);
    std::chrono::seconds ack_poll_interval_ = std::chrono::seconds(10);
    int poll_threads_ = 2;
    std::atomic<bool> started_;
    boost::threadpool::pool workers_;
};

} // namespace splunkhec


#endif //SPLUNK_HEC_CLIENT_CPP_ACK_POLLER_H
