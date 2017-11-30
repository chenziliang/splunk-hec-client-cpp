//
// Created by kchen on 11/29/17.
//

#ifndef SPLUNK_HEC_CLIENT_CPP_EVENT_H
#define SPLUNK_HEC_CLIENT_CPP_EVENT_H

#include <string>
#include <type_traits>

#include "serialize.h"

#include "rapidjson/include/rapidjson/writer.h"


static const std::string sTime = "time";
static const std::string sHost = "host";
static const std::string sIndex = "index";
static const std::string sSource = "source";
static const std::string sSourcetype = "sourcetype";
static const std::string sEvent = "event";

using DefaultWriter = rapidjson::Writer<rapidjson::StringBuffer, rapidjson::UTF8<>, rapidjson::UTF8<>, rapidjson::CrtAllocator, rapidjson::RAPIDJSON_WRITE_DEFAULT_FLAGS>;

template <typename T>
class Event {
public:
    Event(const T& eventData, void* tiedObj):
            event(eventData), tied(tiedObj) {
    }

    Event(T&& eventData, void *tiedObj):
            event(eventData), tied(tiedObj) {
    }

    virtual ~Event() {
    }



    Event& setTime(int64_t t) {
        time = t;
        return *this;
    }

    int64_t getTime() const {
        return time;
    }

    Event& setHost(const std::string& h) {
        host = h;
        return *this;
    }

    const std::string& getHost() const {
        return host;
    }

    Event& setSource(const std::string& s) {
        source = s;
        return *this;
    }

    const std::string& getSource() const {
        return source;
    }

    Event& setSourcetype(const std::string& s) {
        sourcetype = s;
        return *this;
    }

    const std::string& getSourcetype() const {
        return sourcetype;
    }

    Event& setIndex(const std::string& i) {
        index = i;
        return *this;
    }

    const std::string& getIndex() const {
        return index;
    }

    void* getTied() const {
        return tied;
    }

    Event& setLineBreaker(const std::string& breaker) {
        lineBreaker = breaker;
        return *this;
    }

protected:
    int64_t time = -1;
    std::string host;
    std::string index;
    std::string source;
    std::string sourcetype;
    T event;

    std::string lineBreaker = "\n";

    // Event doesn't own tied object
    void *tied;
};

#endif //SPLUNK_HEC_CLIENT_CPP_EVENT_H
