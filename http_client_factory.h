//
// Created by kchen on 12/3/17.
//

#ifndef SPLUNK_HEC_CLIENT_CPP_HTTP_CLIENT_FACTORY_H
#define SPLUNK_HEC_CLIENT_CPP_HTTP_CLIENT_FACTORY_H

#include <memory>
#include <cpprest/http_client.h>

#include "config.h"

namespace splunkhec {

class HttpClientFactory {
public:
    explicit HttpClientFactory(const Config& config) {
        config_.set_chunksize(config.http_buffer_size_);
        config_.set_timeout(config.http_timeout_);
        config_.set_validate_certificates(config.http_validate_certs_);
    }

    std::unique_ptr<web::http::client::http_client> create(const std::string& uri) {
        return std::unique_ptr<web::http::client::http_client>(new web::http::client::http_client(uri, config_));
    }

private:
    web::http::client::http_client_config config_;
};

} // namespace splunkhec

#endif //SPLUNK_HEC_CLIENT_CPP_HTTP_CLIENT_FACTORY_H
