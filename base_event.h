//
// Created by kchen on 11/30/17.
//

#ifndef SPLUNK_HEC_CLIENT_CPP_BASE_EVENT_H
#define SPLUNK_HEC_CLIENT_CPP_BASE_EVENT_H

#include <map>
#include <string>

namespace splunkhec {

class BaseEvent {
public:
    virtual ~BaseEvent() {};
    virtual BaseEvent& set_time(int64_t t) = 0;
    virtual int64_t time() const = 0;
    virtual BaseEvent& set_host(const std::string& h) = 0;
    virtual const std::string& host() const = 0;
    virtual BaseEvent& set_source(const std::string& s) = 0;
    virtual const std::string& source() const = 0;
    virtual BaseEvent& set_sourcetype(const std::string& s) = 0;
    virtual const std::string& sourcetype() const = 0;
    virtual BaseEvent& set_index(const std::string& i) = 0;
    virtual const std::string& index() const = 0;
    virtual void* tied() const = 0;
    virtual BaseEvent& set_linebreaker(const std::string& breaker) = 0;
    virtual BaseEvent& add_fields(const std::map<std::string, std::string>& fields) {
        return *this;
    };
};

} // namespace splunkhec

#endif //SPLUNK_HEC_CLIENT_CPP_BASE_EVENT_H
