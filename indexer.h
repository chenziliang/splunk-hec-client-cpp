//
// Created by kchen on 12/3/17.
//

#ifndef SPLUNK_HEC_CLIENT_CPP_INDEXER_H
#define SPLUNK_HEC_CLIENT_CPP_INDEXER_H

#include <cpprest/http_client.h>

#include "indexer_inf.h"
#include "poller_inf.h"
#include "config.h"
#include "http_client_factory.h"

namespace splunkhec {

class Indexer final: public IndexerInf {
public:
    Indexer(const std::string& base_uri, const std::string& token, PollerInf& poller, const Config& config)
            : base_uri_(base_uri), token_("Splunk " + token), poller_(poller), config_(config), channel_(*this) {
        HttpClientFactory factory(config_);
        client_ = std::move(factory.create(base_uri_));
    }

    void send(const EventBatch& batch) const override {
        web::http::http_request req{web::http::methods::POST};
        auto& headers = req.headers();
        headers.set_content_type(batch.content_type());
        headers.add("X-Splunk-Request-Channel", channel_.id());
        headers.add("Authorization", token_);
        req.set_body(batch.serialize());
        req.set_request_uri(batch.rest_endpoint());

        pplx::task<void> res = client_->request(req).then([&](pplx::task<web::http::http_response> t) {
            try {
                web::http::http_response response = t.get();
                std::string str_resp = response.extract_string().get();
                poller_.add(channel_, batch, str_resp);
            } catch (const web::http::http_exception& ex) {
                poller_.fail(channel_, batch, ex);
            }
        });
        res.get();
    }

    bool has_backpressure() const override {
        return false;
    }

private:
    std::string base_uri_;
    std::string token_;
    PollerInf& poller_;
    Config config_;
    std::unique_ptr<web::http::client::http_client> client_;
    HecChannel channel_;
};

} // namespace splunkhec

#endif //SPLUNK_HEC_CLIENT_CPP_INDEXER_H
