//
// Created by kchen on 11/30/17.
//

#ifndef SPLUNK_HEC_CLIENT_CPP_RAW_EVENT_H
#define SPLUNK_HEC_CLIENT_CPP_RAW_EVENT_H

#include <string>

#include "event.h"

template <typename T>
class RawEvent: public Event<T> {
public:
    RawEvent(const T& eventData, void* tiedObj): Event<T>(eventData, tiedObj) {
        Event<T>::setLineBreaker("");
    }

    RawEvent(T&& eventData, void *tiedObj): Event<T>(eventData, tiedObj) {
        Event<T>::setLineBreaker("");
    }

    template <typename Writer>
    void serialize(Writer& writer) const {
        ::serialize(writer, Event<T>::event);
    }
};

#endif //SPLUNK_HEC_CLIENT_CPP_RAW_EVENT_H
