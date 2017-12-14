//
// Created by kchen on 12/12/17.
//

#pragma once

#ifndef SPLUNK_HEC_CLIENT_CPP_HEC_RESPONSE_PARSER_H
#define SPLUNK_HEC_CLIENT_CPP_HEC_RESPONSE_PARSER_H

#include <string>
#include <vector>

namespace splunkhec {

// return code
int parse_response(const std::string& response);

// return {code, ack_id}
std::pair<int, int64_t> parse_response_with_ackid(const std::string& response);

// return true for success, false for parse failure
bool parse_ack_poll_response(const std::string& response, std::vector<int64_t>& committed_acks);

std::vector<unsigned char> prepare_ack_poll_payload(const std::vector<int64_t>& ack_ids);

} // namespace splunkhec

#endif //SPLUNK_HEC_CLIENT_CPP_HEC_RESPONSE_PARSER_H
