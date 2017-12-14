//
// Created by kchen on 12/3/17.
//

#include "ack_poller.h"
#include "hec_exception.h"
#include "hec_response_parser.h"

using namespace std;
using namespace rapidjson;
using namespace concurrentcpp;

namespace splunkhec {

AckPoller::AckPoller(size_t threads,  const shared_ptr<PollerCallbackInf>& callback)
        : pool_(threads + 1), callback_(callback), logger_(spdlog::get("splunk-hec")) {
}

void AckPoller::start() {
    bool started = false;
    bool exchanged = started_.compare_exchange_strong(started, true);
    if (!exchanged) {
        // already started
        return;
    }
    pool_.submit(bind(&AckPoller::poll, this));
    logger_->info("AckPoller started");
}

void AckPoller::stop() {
    bool started = false;
    bool exchanged = started_.compare_exchange_strong(started, true);
    if (exchanged) {
        // not started
        return;
    }
}

void AckPoller::poll() {
    for (;started_.load();) {
        this_thread::sleep_for(poll_interval_);
        do_poll();
    }
}

void AckPoller::do_poll() {
    vector<pair<shared_ptr<IndexerInf>, vector<int64_t>>> inflight_ack_ids;
    vector<shared_ptr<EventBatch>> timeouts;

    {
        lock_guard<mutex> guard{lock_};
        for (auto& kv: outstanding_batches_) {
            vector<int64_t> inflights;
            kv.second->categorize(inflights, timeouts, ttl_);
            if (!inflights.empty()) {
                inflight_ack_ids.push_back(make_pair(kv.second->indexer(), move(inflights)));
            }
        }
    }

    if (!timeouts.empty() && callback_) {
        logger_->warn("handling {} timedout batches", timeouts.size());
        callback_->on_event_failed(timeouts, HecException("timed out", -1));
    }

    for (auto& indexer_acks: inflight_ack_ids) {
        if (!started_.load()) {
            return;
        }

        for (;started_.load();) {
            try {
                pool_.submit(bind(&AckPoller::poll_acks, this, move(indexer_acks.first), move(indexer_acks.second)));
            } catch (const TimeoutException& e) {
                continue;
                // pool can't handle more
                // just retry unless it is shutting down
            }
            break;
        }
    }
}

void AckPoller::poll_acks(const shared_ptr<IndexerInf>& indexer, const vector<int64_t>& ack_ids) {
    if (ack_ids.empty()) {
        return;
    }

    logger_->info("polling {} acks from {} for channel {}", ack_ids.size(), indexer->uri(), indexer->channel());
    vector<unsigned char> bytes{prepare_ack_poll_payload(ack_ids)};
    try {
        string resp = indexer->post(ack_endpoint_, bytes, "application/json");
        handle_ack_poll_response(resp, indexer);
    } catch (const HecException& e) {
        logger_->error("failed to poll ack from {} for channel {}, error={}",
                       indexer->uri(), indexer->channel(), e.what());
    }
}

void AckPoller::handle_ack_poll_response(const string& resp, const shared_ptr<IndexerInf>& indexer) {
    vector<int64_t> committed_acks;
    bool ok = parse_ack_poll_response(resp, committed_acks);
    if (!ok) {
        logger_->error("failed to poll ackes from {} for channel {}, response={}",
                       indexer->uri(), indexer->channel(), resp);
        return;
    }

    logger_->info("polled {} ackes from {} for channel {}",
                  committed_acks.size(), indexer->uri(), indexer->channel());
    if (committed_acks.empty()) {
        return;
    }

    shared_ptr<OutstandingBatches> batches;
    {
        lock_guard<mutex> guard{lock_};
        batches = outstanding_batches_[indexer->channel()];
    }

    if (batches) {
        batches->commit(committed_acks);
    }
}

void AckPoller::add(const shared_ptr<IndexerInf>& indexer, const shared_ptr<EventBatch>& batch, const string& response) {
    auto code_with_ackid = parse_response_with_ackid(response);
    if (code_with_ackid.first != 0) {
        logger_->error("failed to post events to {} for channel {}, response={}",
                       indexer->uri(), indexer->channel(), response);
        return;
    }

    shared_ptr<OutstandingBatches> batches;
    {
        lock_guard<mutex> guard{lock_};
        auto it = outstanding_batches_.find(indexer->channel());
        if (it != outstanding_batches_.end()) {
            batches = it->second;
        } else {
            // create OutstandingBatches
            batches = make_shared<OutstandingBatches>(indexer, this);
            outstanding_batches_.insert({indexer->channel(), batches});
        }
    }

    batches->add(code_with_ackid.second, batch);
}

void AckPoller::fail(const shared_ptr<IndexerInf>& indexer, const shared_ptr<EventBatch>& batch, const exception& ex) {
    batch->fail();
    if (callback_) {
        callback_->on_event_failed(vector<shared_ptr<EventBatch>>{batch}, ex);
    }
}

const string AckPoller::ack_endpoint_ = "/services/collector/ack";

} // namespace splunkhec
