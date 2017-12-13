//
// Created by kchen on 12/12/17.
//

#ifndef SPLUNK_HEC_CLIENT_CPP_HEC_RESPONSE_PARSER_H
#define SPLUNK_HEC_CLIENT_CPP_HEC_RESPONSE_PARSER_H

#include "serialize.h"
#include "rapidjson/include/rapidjson/document.h"

#include <utility>
#include <string>
#include <vector>
#include <cstring>

namespace splunkhec {

static int do_parse_response(rapidjson::Document& doc, const std::string response) {
    doc.Parse(response.data(), response.size());
    if (doc.HasParseError()) {
        return -1;
    }

    auto it{doc.FindMember("code")};
    if (it == doc.MemberEnd() || !it->value.IsInt()) {
        return -1;
    }

    return it->value.GetInt();
}

// return code
int parse_response(const std::string& response) {
    // {"text":"Success","code":0}
    rapidjson::Document doc;
    return do_parse_response(doc, response);
}

// return {code, ack_id}
std::pair<int, int64_t> parse_response_with_ackid(const std::string& response) {
    // {"text":"Success","code":0, "ackId":7}

    rapidjson::Document doc;
    int code = do_parse_response(doc, response);
    if (code != 0) {
        return std::make_pair(code, -1);
    }

    auto it{doc.FindMember("ackId")};
    if (it == doc.MemberEnd() || !it->value.IsInt64()) {
        return std::make_pair(code, -1);
    }

    return std::make_pair(code, it->value.GetInt64());
}

// return true for success, false for parse failure
bool parse_ack_poll_response(const std::string& response, std::vector<int64_t>& committed_acks) {
    // {"acks":{"8":false,"9":false,"10":false}}

    rapidjson::Document doc;
    doc.Parse(response.data(), response.size());
    if (doc.HasParseError()) {
        return false;
    }

    auto pos{doc.FindMember("acks")};
    if (pos == doc.MemberEnd() || !pos->value.IsObject()) {
        return false;
    }

    for (auto it = pos->value.MemberBegin(); it != pos->value.MemberEnd(); ++it) {
        if (!it->value.IsBool()) {
            // TODO log
            continue;
        }

        if (it->value.GetBool() && it->name.IsString()) {
            // committed
            int64_t ack_id = strtoll(it->name.GetString(), 0, 10);
            committed_acks.push_back(ack_id);
        }
    }

    return true;
};

std::vector<unsigned char> prepare_ack_poll_payload(const std::vector<int64_t>& ack_ids) {
    // {"acks":[10,9,8]}
    DefaultStringBuffer buffer;
    DefaultWriter writer(buffer);

    writer.StartObject();
    writer.String("acks");
    writer.StartArray();
    for (int64_t ack_id: ack_ids) {
        serialize(writer, ack_id);
    }
    writer.EndArray();
    writer.EndObject();

    std::vector<unsigned char> bytes;
    bytes.resize(buffer.GetSize());
    memcpy(&bytes[0], buffer.GetString(), buffer.GetSize());

    return bytes;
}

} // namespace splunkhec

#endif //SPLUNK_HEC_CLIENT_CPP_HEC_RESPONSE_PARSER_H
