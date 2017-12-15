//
// Created by kchen on 12/3/17.
//

#pragma once

#ifndef SPLUNK_HEC_CLIENT_CPP_ACK_POLLER_H
#define SPLUNK_HEC_CLIENT_CPP_ACK_POLLER_H

#include "poller_inf.h"
#include "serialize.h"
#include "poller_callback_inf.h"
#include "indexer_inf.h"
#include "concurrent/thread_pool.h"

#include <spdlog/spdlog.h>

#include <memory>
#include <string>
#include <exception>
#include <unordered_map>
#include <chrono>
#include <atomic>
#include <mutex>
#include <cstring>

namespace splunkhec {

class HecChannel;
class EventBatch;

class AckPoller final: public PollerInf {
public:
    AckPoller(std::size_t threads, const std::shared_ptr<PollerCallbackInf>& callback);
    ~AckPoller() {
        stop();
    }

    void start() override;
    void stop() override;
    void add(const std::shared_ptr<IndexerInf>& indexer, const std::shared_ptr<EventBatch>& batch, const std::string& response) override;
    void fail(const std::shared_ptr<IndexerInf>& indexer, const std::shared_ptr<EventBatch>& batch, const std::exception& ex) override;

    AckPoller& set_event_batch_ttl(const std::chrono::seconds& ttl) {
        ttl_ = ttl;
        return *this;
    }

    AckPoller& set_ack_poll_interval(const std::chrono::seconds& interval) {
        poll_interval_ = interval;
        return *this;
    }

    AckPoller& operator=(const AckPoller&) = delete;
    AckPoller& operator=(AckPoller&&) = delete;
    AckPoller(const AckPoller&) = delete;
    AckPoller(AckPoller&&) = delete;

private:
    void poll();
    void do_poll();

    void poll_acks(const std::shared_ptr<IndexerInf>& indexer, const std::vector<int64_t>& ack_ids);
    void handle_ack_poll_response(const std::string& resp,  const std::shared_ptr<IndexerInf>& indexer);

private:
    static const std::string ack_endpoint_;

private:
    struct OutstandingBatches {
    public:
        OutstandingBatches(const std::shared_ptr<IndexerInf>& indexer, AckPoller* poller): indexer_(indexer), lock_(new std::mutex()) {
        }

        void add(int64_t ack_id, const std::shared_ptr<EventBatch>& batch) {
            std::lock_guard<std::mutex> guard{*lock_};
            batches_.insert({ack_id, batch});
        }

        // categorize event batches into inflights and timedouts
        // for timedouts, they are also removed from intenral batches
        void categorize(std::vector<int64_t>& inflights, std::vector<std::shared_ptr<EventBatch>>& timeouts, const std::chrono::seconds& ttl) {
            std::lock_guard<std::mutex> guard{*lock_};
            auto end = batches_.end();
            for (auto it = batches_.begin(); it != end; ++it) {
                if (it->second->timedout(ttl)) {
                    timeouts.push_back(it->second);
                    batches_.erase(it);
                } else {
                    inflights.push_back(it->first);
                }
            }
        }

        std::shared_ptr<IndexerInf> indexer() const {
            return indexer_;
        }

        void commit(const std::vector<int64_t>& acks) {
            if (acks.empty()) {
                return;
            }

            std::lock_guard<std::mutex> guard{*lock_};
            auto end{batches_.end()};
            for (auto ack: acks) {
                auto it{batches_.find(ack)};
                if (it != end) {
                    it->second->commit();
                    batches_.erase(it);
                }
            }
        }

    private:
        std::shared_ptr<IndexerInf> indexer_;
        std::unique_ptr<std::mutex> lock_;
        std::unordered_map<int64_t, std::shared_ptr<EventBatch>> batches_;
    };

private:
    std::mutex lock_;
    std::unordered_map<std::string, std::shared_ptr<OutstandingBatches>> outstanding_batches_;
    std::shared_ptr<PollerCallbackInf> callback_;
    std::chrono::seconds ttl_ = std::chrono::seconds(120);
    std::chrono::seconds poll_interval_ = std::chrono::seconds(10);
    concurrentcpp::ThreadPool pool_;
    std::atomic_bool started_;

private:
    std::shared_ptr<spdlog::logger> logger_;
};

} // namespace splunkhec


#endif //SPLUNK_HEC_CLIENT_CPP_ACK_POLLER_H
