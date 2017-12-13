//
// Created by kchen on 11/30/17.
//

#ifndef SPLUNK_HEC_CLIENT_CPP_RAW_EVENT_BATCH_H
#define SPLUNK_HEC_CLIENT_CPP_RAW_EVENT_BATCH_H

#include "event_batch.h"

#include <string>
#include <vector>

namespace splunkhec {

class RawEventBatch final: public EventBatch {
public:
    RawEventBatch(const std::string& index, const std::string& source,
                  const std::string& sourcetype, const std::string& host, int64_t time=-1);

    const std::string& index() const {
        return index_;
    }

    const std::string& source() const {
        return source_;
    }

    const std::string& sourcetype() const {
        return sourcetype_;
    }

    const std::string& host() const {
        return host_;
    }

    int64_t time() const {
        return time_;
    }

    std::string rest_endpoint() const override;
    std::string content_type() const override;
    RawEventBatch* create_from_this() const override;

    class Builder {
    public:
        Builder& set_index(const std::string& index) {
            index_ = index;
            return *this;
        }

        Builder& set_source(const std::string& source) {
            source_ = source;
            return *this;
        }

        Builder& set_sourcetype(const std::string& sourcetype) {
            sourcetype_ = sourcetype;
            return *this;
        }

        Builder& set_host(const std::string& host) {
            host_ = host;
            return *this;
        }

        Builder& set_time(int64_t time) {
            time_ = time;
            return *this;
        }

        RawEventBatch* build() {
            return new RawEventBatch(index_, source_, sourcetype_, host_, time_);
        }

    private:
        std::string index_;
        std::string source_;
        std::string sourcetype_;
        std::string host_;
        int64_t time_ = -1;
    };

private:
    std::string metadata_params() const;

private:
    static const std::string raw_endpoint;
    static const std::string raw_content_type;

private:
    std::string index_;
    std::string source_;
    std::string sourcetype_;
    std::string host_;
    int64_t time_ = -1;
};

} // namespace splunkhec

#endif //SPLUNK_HEC_CLIENT_CPP_RAW_EVENT_BATCH_H
