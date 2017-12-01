//
// Created by kchen on 11/30/17.
//

#ifndef SPLUNK_HEC_CLIENT_CPP_EVENT_BATCH_H
#define SPLUNK_HEC_CLIENT_CPP_EVENT_BATCH_H

#include <chrono>
#include <map>
#include <memory>
#include <vector>

#include "base_event.h"
#include "event.h"

namespace splunkhec {

enum EventBatchStatus {
    INIT,
    COMMITTED,
    FAILED
};

using EventContainer = std::vector<std::unique_ptr<BaseEvent>>;

class EventBatch {
public:
    virtual ~EventBatch() {}
    virtual std::string rest_endpoint() const = 0;
    virtual std::string content_type() const = 0;
    virtual EventBatch* create_from_this() const = 0;

    void add(BaseEvent* event) {
        assert (event != nullptr);
        events_.emplace_back(event);
    }

    EventBatch& add_extra_fields(const std::map<std::string, std::string>& fields) {
        for (auto& event: events_) {
            event->add_fields(fields);
        }
        return *this;
    }

    bool timedout(int ttl /* in seconds */) const {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - send_timestamp_).count();
        return ttl > elapsed;
    }

    void reset_sendtimestamp() {
        send_timestamp_ = std::chrono::steady_clock::now();
    }

    bool committed() const {
        return status_ == COMMITTED;
    }

    bool failed() const {
        return status_ == FAILED;
    }

    EventBatch& init() {
        status_ = INIT;
        return *this;
    }

    EventBatch& fail() {
        status_ = FAILED;
        ++failure_count_;
        return *this;
    }

    EventBatch& commit() {
        status_ = COMMITTED;
        return *this;
    }

    int failure_count() const {
        return failure_count_;
    }

    const EventContainer& events() const {
        return events_;
    }

    std::size_t size() const {
        return events_.size();
    }

    std::size_t size_in_bytes() const {
        return len_;
    }


    bool empty() const {
        return events_.empty();
    }

protected:
    std::size_t len_;
    EventContainer events_;

private:
    volatile EventBatchStatus status_ = INIT;
    int failure_count_ = 0;
    std::chrono::steady_clock::time_point send_timestamp_ = std::chrono::steady_clock::now();
};

} // namespace splunkhec

#endif //SPLUNK_HEC_CLIENT_CPP_EVENT_BATCH_H
