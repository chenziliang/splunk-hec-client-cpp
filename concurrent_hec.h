//
// Created by kchen on 12/13/17.
//

#pragma once

#ifndef SPLUNK_HEC_CLIENT_CPP_CONCURRENT_HEC_H
#define SPLUNK_HEC_CLIENT_CPP_CONCURRENT_HEC_H

#include "hec_inf.h"
#include "event_batch.h"
#include "poller_callback_inf.h"
#include "concurrent/thread_pool.h"
#include "config.h"

#include <memory>
#include <atomic>
#include <vector>

namespace splunkhec {

class ConcurrentHec final: public HecInf {
public:
    ConcurrentHec(std::size_t threads, bool use_ack, Config& config, const std::shared_ptr<PollerCallbackInf>& callback);
    bool send(const std::shared_ptr<EventBatch>& batch) override;
    void stop() override;

    ConcurrentHec(const ConcurrentHec&) = delete;
    ConcurrentHec& operator=(const ConcurrentHec&) = delete;
    ConcurrentHec(ConcurrentHec&&) = delete;
    ConcurrentHec& operator=(ConcurrentHec&&) = delete;

private:
    void do_send(int idx);
    void new_hecs(std::size_t num, bool use_ack, const Config& config, const std::shared_ptr<PollerCallbackInf>& callback);

private:
    concurrentcpp::ThreadPool workers_;
    std::vector<std::shared_ptr<HecInf>> hecs_;
    concurrentcpp::BlockingQueue<std::shared_ptr<EventBatch>> batches_;
    std::atomic_bool stopped_;
};

}

#endif //SPLUNK_HEC_CLIENT_CPP_CONCURRENT_HEC_H
