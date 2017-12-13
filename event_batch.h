//
// Created by kchen on 11/30/17.
//

#ifndef SPLUNK_HEC_CLIENT_CPP_EVENT_BATCH_H
#define SPLUNK_HEC_CLIENT_CPP_EVENT_BATCH_H

#include "event_inf.h"

#include <chrono>
#include <map>
#include <memory>
#include <vector>
#include <string>

namespace splunkhec {

enum EventBatchStatus {
    INIT,
    COMMITTED,
    FAILED
};

using EventContainer = std::vector<std::unique_ptr<EventInf>>;

class EventBatch {
public:
    virtual ~EventBatch() {}
    virtual std::string rest_endpoint() const = 0;
    virtual std::string content_type() const = 0;
    virtual EventBatch* create_from_this() const = 0;

    std::vector<unsigned char> serialize() const;

    void add(EventInf* event);

    EventBatch& add_extra_fields(const std::map<std::string, std::string>& fields);

    bool timedout(const std::chrono::seconds& ttl) const {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - send_timestamp_);
        return elapsed > ttl;
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

    EventBatch& set_linebreaker(const std::string& breaker) {
        line_breaker_ = breaker;
        return *this;
    }

    const std::string& line_breaker() const {
        return line_breaker_;
    }

    bool empty() const {
        return events_.empty();
    }

protected:
    EventContainer events_;

private:
    std::string line_breaker_ = "\n";

private:
    volatile EventBatchStatus status_ = INIT;
    int failure_count_ = 0;
    std::chrono::steady_clock::time_point send_timestamp_ = std::chrono::steady_clock::now();
};

} // namespace splunkhec

#endif //SPLUNK_HEC_CLIENT_CPP_EVENT_BATCH_H
