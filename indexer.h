//
// Created by kchen on 12/3/17.
//

#ifndef SPLUNK_HEC_CLIENT_CPP_INDEXER_H
#define SPLUNK_HEC_CLIENT_CPP_INDEXER_H

#include "indexer_inf.h"
#include "http_client_factory.h"

#include <cpprest/http_client.h>
#include <spdlog/spdlog.h>

#include <string>
#include <memory>
#include <chrono>

namespace splunkhec {

class PollerInf;

class Indexer final: public IndexerInf, public std::enable_shared_from_this<Indexer> {
public:
    Indexer(const std::string& base_uri, const std::string& token, const std::shared_ptr<PollerInf>& poller, const HttpClientFactory& factory);

    bool send(const std::shared_ptr<EventBatch>& batch) override;

    std::string post(const std::string& uri, const std::vector<unsigned char>& data, const std::string& content_type) override;

    bool has_backpressure() const override;

    const std::string& channel() const override {
        return channel_;
    }

    const std::string& uri() const override {
        return uri_;
    }

private:
    void clear_backpressure() {
        backpressure_ = 0;
        last_backpressure_time_ = std::chrono::steady_clock::time_point();
    }

    void log_backpressure() {
        ++backpressure_;
        last_backpressure_time_ = std::chrono::steady_clock::now();
    }

private:
    static const std::chrono::seconds backpressure_window_;

private:
    std::string uri_;
    std::string token_;
    std::string channel_;
    std::shared_ptr<PollerInf> poller_;
    std::unique_ptr<web::http::client::http_client> client_;
    HttpClientFactory factory_;
    int backpressure_;
    std::chrono::steady_clock::time_point last_backpressure_time_;

private:
    std::shared_ptr<spdlog::logger> logger_;
};

} // namespace splunkhec

#endif //SPLUNK_HEC_CLIENT_CPP_INDEXER_H
