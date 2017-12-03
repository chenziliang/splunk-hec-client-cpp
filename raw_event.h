//
// Created by kchen on 11/30/17.
//

#ifndef SPLUNK_HEC_CLIENT_CPP_RAW_EVENT_H
#define SPLUNK_HEC_CLIENT_CPP_RAW_EVENT_H

#include <string>

#include "serialize.h"
#include "event.h"

namespace splunkhec {

template <typename T>
class RawEvent final: public Event<T> {
public:
    RawEvent(const T& eventData, void* tiedObj): Event<T>(eventData, tiedObj) {
    }

    RawEvent(T&& eventData, void *tiedObj): Event<T>(eventData, tiedObj) {
    }

    void serialize(DefaultStringBuffer& buffer) const override {
        DefaultWriter writer(buffer);
        splunkhec::serialize(writer, Event<T>::event_);
    }
};

} // namespace splunkhec

#endif //SPLUNK_HEC_CLIENT_CPP_RAW_EVENT_H
