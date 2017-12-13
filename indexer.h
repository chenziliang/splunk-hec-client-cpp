//
// Created by kchen on 12/3/17.
//

#ifndef SPLUNK_HEC_CLIENT_CPP_INDEXER_H
#define SPLUNK_HEC_CLIENT_CPP_INDEXER_H

#include "indexer_inf.h"
#include "http_client_factory.h"

#include <cpprest/http_client.h>

#include <string>
#include <memory>

namespace splunkhec {

class PollerInf;

class Indexer final: public IndexerInf, public std::enable_shared_from_this<Indexer> {
public:
    Indexer(const std::string& base_uri, const std::string& token, const std::shared_ptr<PollerInf>& poller, const HttpClientFactory& factory);
    void send(const std::shared_ptr<EventBatch>& batch) const override;
    std::string post(const std::string& uri, const std::vector<unsigned char>& data, const std::string& content_type) const override;
    bool has_backpressure() const override {
        return false;
    }
    std::string channel() const override {
        return channel_;
    }

private:
    std::string token_;
    std::string channel_;
    std::shared_ptr<PollerInf> poller_;
    std::unique_ptr<web::http::client::http_client> client_;
    HttpClientFactory factory_;
};

} // namespace splunkhec

#endif //SPLUNK_HEC_CLIENT_CPP_INDEXER_H
