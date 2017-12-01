//
// Created by kchen on 11/30/17.
//

#ifndef SPLUNK_HEC_CLIENT_CPP_JSON_EVENT_H_H
#define SPLUNK_HEC_CLIENT_CPP_JSON_EVENT_H_H

#include <map>
#include <string>

#include "event.h"

namespace splunkhec {

static const std::string sFields = "fields";

template <typename T>
class JsonEvent: public Event<T> {
public:
    JsonEvent(const T& eventData, void* tiedObj): Event<T>(eventData, tiedObj) {
    }

    JsonEvent(T&& eventData, void *tiedObj): Event<T>(eventData, tiedObj) {
    }

    JsonEvent& add_fields(const std::map<std::string, std::string>& extraFields) {
        for(const auto& k: extraFields) {
            fields[k.first] = k.second;
        }
        return *this;
    }

    JsonEvent& set_fields(const std::map<std::string, std::string>& extraFields) {
        fields = extraFields;
        return *this;
    }

#define WRITE_IF_NOT_EMPTY(k, v) \
    if (!v.empty()) { \
        writer.String(k.data(), k.size()); \
        writer.String(v.data(), v.size()); \
    }

    template <typename Writer>
    void serialize(Writer& writer) const {
        writer.StartObject();

        if (Event<T>::time_ > 0) {
            writer.String(sTime.data(), sTime.size());
            writer.Int64(Event<T>::time_);
        }

        WRITE_IF_NOT_EMPTY(sHost, Event<T>::host_);
        WRITE_IF_NOT_EMPTY(sIndex, Event<T>::index_);
        WRITE_IF_NOT_EMPTY(sSource, Event<T>::source_);
        WRITE_IF_NOT_EMPTY(sSourcetype, Event<T>::sourcetype_);

        serialize_fields(writer);

        writer.String(sEvent.data(), sEvent.size());
        splunkhec::serialize(writer, Event<T>::event_);

        writer.EndObject();
    }

private:
    template <typename Writer>
    void serialize_fields(Writer& writer) const {
        writer.String(sFields.data(), sFields.size());
        writer.StartObject();
        for (const auto& kv: fields) {
            writer.String(kv.first.data(), kv.first.size());
            writer.String(kv.second.data(), kv.second.size());
        }
        writer.EndObject();
    }

private:
    std::map<std::string, std::string> fields;
};

} // namespace splunkhec

#endif //SPLUNK_HEC_CLIENT_CPP_JSON_EVENT_H_H
