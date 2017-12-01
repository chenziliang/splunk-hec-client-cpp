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
class RawEvent: public Event<T> {
public:
    RawEvent(const T& eventData, void* tiedObj): Event<T>(eventData, tiedObj) {
        Event<T>::set_linebreaker("");
    }

    RawEvent(T&& eventData, void *tiedObj): Event<T>(eventData, tiedObj) {
        Event<T>::set_linebreaker("");
    }

    template <typename Writer>
    void serialize(Writer& writer) const {
        splunkhec::serialize(writer, Event<T>::event_);
    }
};

} // namespace splunkhec

#endif //SPLUNK_HEC_CLIENT_CPP_RAW_EVENT_H
