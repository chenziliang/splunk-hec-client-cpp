//
// Created by kchen on 11/30/17.
//

#ifndef SPLUNK_HEC_CLIENT_CPP_BASE_EVENT_H
#define SPLUNK_HEC_CLIENT_CPP_BASE_EVENT_H

#include "rapidjson/include/rapidjson/writer.h"
#include "rapidjson/include/rapidjson/stringbuffer.h"

#include <boost/any.hpp>

#include <map>
#include <string>
#include <cstring>

namespace splunkhec {

using DefaultWriter = rapidjson::Writer<rapidjson::StringBuffer, rapidjson::UTF8<>, rapidjson::UTF8<>, rapidjson::CrtAllocator, rapidjson::RAPIDJSON_WRITE_DEFAULT_FLAGS>;
using DefaultStringBuffer = rapidjson::StringBuffer;

class EventInf {
public:
    virtual ~EventInf() {};
    virtual EventInf& set_time(int64_t t) = 0;
    virtual int64_t time() const = 0;
    virtual EventInf& set_host(const std::string& h) = 0;
    virtual const std::string& host() const = 0;
    virtual EventInf& set_source(const std::string& s) = 0;
    virtual const std::string& source() const = 0;
    virtual EventInf& set_sourcetype(const std::string& s) = 0;
    virtual const std::string& sourcetype() const = 0;
    virtual EventInf& set_index(const std::string& i) = 0;
    virtual const std::string& index() const = 0;
    virtual const boost::any& tied() const = 0;
    virtual void serialize(DefaultStringBuffer& buffer) const = 0;

    virtual EventInf& add_fields(const std::map<std::string, std::string>& fields) {
        return *this;
    };

    virtual EventInf& set_fields(const std::map<std::string, std::string>& extraFields) {
        return *this;
    }

    std::string string() const {
        rapidjson::StringBuffer buffer;
        serialize(buffer);
        return std::string(buffer.GetString());
    };
};

} // namespace splunkhec

#endif //SPLUNK_HEC_CLIENT_CPP_BASE_EVENT_H
