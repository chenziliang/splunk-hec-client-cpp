//
// Created by kchen on 12/2/17.
//

#ifndef SPLUNK_HEC_CLIENT_CPP_CONFIG_H
#define SPLUNK_HEC_CLIENT_CPP_CONFIG_H

#include <chrono>

namespace splunkhec {

struct Config {
    bool http_validate_certs_ = true;
    std::chrono::seconds http_timeout_ = std::chrono::seconds(120);
    std::size_t http_buffer_size_ = 8 * 1024 * 1024;
};

} // namespace splunkhec

#endif //SPLUNK_HEC_CLIENT_CPP_CONFIG_H
