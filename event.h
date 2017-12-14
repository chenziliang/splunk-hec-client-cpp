//
// Created by kchen on 11/29/17.
//

#pragma once

#ifndef SPLUNK_HEC_CLIENT_CPP_EVENT_H
#define SPLUNK_HEC_CLIENT_CPP_EVENT_H

#include "serialize.h"
#include "event_inf.h"

#include <string>
#include <type_traits>

namespace splunkhec {

static const std::string sTime = "time";
static const std::string sHost = "host";
static const std::string sIndex = "index";
static const std::string sSource = "source";
static const std::string sSourcetype = "sourcetype";
static const std::string sEvent = "event";

// If T are builint types: integer, double, string, char*, bool
// then the serialization just works
// If T is object or array, clients need wrap object or array to
// implements void serialize(Writer& writer) interface
template <typename T>
class Event: public EventInf {
public:
    Event(const T& event, const boost::any& tied):
            event_(event), tied_(tied) {
    }

    Event(T&& event, const boost::any& tied):
            event_(event), tied_(tied) {
    }

    virtual ~Event() {
    }

    Event& set_time(int64_t t) final {
        time_ = t;
        return *this;
    }

    int64_t time() const final {
        return time_;
    }

    Event& set_host(const std::string& h) final {
        host_ = h;
        return *this;
    }

    const std::string& host() const final {
        return host_;
    }

    Event& set_source(const std::string& s) final {
        source_ = s;
        return *this;
    }

    const std::string& source() const final {
        return source_;
    }

    Event& set_sourcetype(const std::string& s) final {
        sourcetype_ = s;
        return *this;
    }

    const std::string& sourcetype() const final {
        return sourcetype_;
    }

    Event& set_index(const std::string& i) final {
        index_ = i;
        return *this;
    }

    const std::string& index() const final {
        return index_;
    }

    const boost::any& tied() const final {
        return tied_;
    }

protected:
    int64_t time_ = -1;
    std::string host_;
    std::string index_;
    std::string source_;
    std::string sourcetype_;
    T event_;

    // Event doesn't own tied object
    boost::any tied_;
};

} // namespace splunkhec

#endif //SPLUNK_HEC_CLIENT_CPP_EVENT_H
