//
// Created by kchen on 11/29/17.
//

#ifndef SPLUNK_HEC_CLIENT_CPP_SERIALIZE_H
#define SPLUNK_HEC_CLIENT_CPP_SERIALIZE_H

#include "rapidjson/include/rapidjson/writer.h"
#include "rapidjson/include/rapidjson/stringbuffer.h"

#include <string>
#include <cstddef>

namespace splunkhec {

using DefaultWriter = rapidjson::Writer<rapidjson::StringBuffer, rapidjson::UTF8<>, rapidjson::UTF8<>, rapidjson::CrtAllocator, rapidjson::RAPIDJSON_WRITE_DEFAULT_FLAGS>;
using DefaultStringBuffer = rapidjson::StringBuffer;

template <typename T, typename Writer>
void serialize(Writer& writer, const T& t) {
    t.serialize(writer);
};

template <typename Writer>
void serialize(Writer& writer, int t) {
    writer.Int(t);
};

template <typename Writer>
void serialize(Writer& writer, unsigned t) {
    writer.Uint(t);
};

template <typename Writer>
void serialize(Writer& writer, int64_t t) {
    writer.Int64(t);
};

template <typename Writer>
void serialize(Writer& writer, uint64_t t) {
    writer.Uint64(t);
};

template <typename Writer>
void serialize(Writer& writer, bool t) {
    writer.Bool(t);
};

template <typename Writer>
void serialize(Writer& writer, std::nullptr_t t) {
    writer.Null();
};

template <typename Writer>
void serialize(Writer& writer, double t) {
    writer.Double(t);
};

template <typename Writer>
void serialize(Writer& writer, const char* t) {
    writer.String(t, strlen(t));
};

template <typename Writer>
void serialize(Writer& writer, const std::string& t) {
    writer.String(t.data(), t.size());
};

} // namespace splunkhec

#endif //SPLUNK_HEC_CLIENT_CPP_SERIALIZE_H
