//
// Created by kchen on 12/2/17.
//

#ifndef SPLUNK_HEC_CLIENT_CPP_CONFIG_H
#define SPLUNK_HEC_CLIENT_CPP_CONFIG_H

#include <chrono>
#include <string>
#include <vector>

namespace splunkhec {

struct Config {
public:
    Config(const std::vector<std::string>& uris, const std::string& token)
            : uris_(uris), token_(token) {
    }

    std::vector<std::string> uris_;
    std::string token_;

    int ack_poll_threads_ = 2;
    int total_channels_ = 2;
    bool http_validate_certs_ = true;
    bool enable_channel_tracking = false;
    std::chrono::seconds http_timeout_ = std::chrono::seconds(120);
    std::size_t http_buffer_size_ = 8 * 1024 * 1024;
};

} // namespace splunkhec

#endif //SPLUNK_HEC_CLIENT_CPP_CONFIG_H
