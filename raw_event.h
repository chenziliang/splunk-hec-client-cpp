//
// Created by kchen on 11/30/17.
//

#pragma once

#ifndef SPLUNK_HEC_CLIENT_CPP_RAW_EVENT_H
#define SPLUNK_HEC_CLIENT_CPP_RAW_EVENT_H

#include "serialize.h"
#include "event.h"

#include <string>

namespace splunkhec {

template <typename T>
class RawEvent final: public Event<T> {
public:
    RawEvent(const T& event_data, const boost::any& tied_obj): Event<T>(event_data, tied_obj) {
    }

    RawEvent(T&& event_data, boost::any&& tied_obj): Event<T>(std::move(event_data), std::move(tied_obj)) {
    }

    void serialize(DefaultStringBuffer& buffer) const override {
        DefaultWriter writer(buffer);
        splunkhec::serialize(writer, Event<T>::event_);
    }
};

} // namespace splunkhec

#endif //SPLUNK_HEC_CLIENT_CPP_RAW_EVENT_H
