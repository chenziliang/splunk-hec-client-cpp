//
// Created by kchen on 12/3/17.
//

#ifndef SPLUNK_HEC_CLIENT_CPP_HTTP_CLIENT_FACTORY_H
#define SPLUNK_HEC_CLIENT_CPP_HTTP_CLIENT_FACTORY_H

#include <memory>
#include <cpprest/http_client.h>

namespace splunkhec {

class HttpClientFactory {
public:
    HttpClientFactory() = default;

    HttpClientFactory& set_chunksize(std::size_t chunk_size) {
        config_.set_chunksize(chunk_size);
        return *this;
    }

    HttpClientFactory& set_timeout(std::chrono::seconds timeout) {
        config_.set_timeout(timeout);
        return *this;
    }

    HttpClientFactory& set_validate_certificates(bool validate_certs) {
        config_.set_validate_certificates(validate_certs);
        return *this;
    }

    web::http::client::http_client* create(const std::string& uri) const {
        return new web::http::client::http_client(uri, config_);
    }

private:
    web::http::client::http_client_config config_;
};

} // namespace splunkhec

#endif //SPLUNK_HEC_CLIENT_CPP_HTTP_CLIENT_FACTORY_H
