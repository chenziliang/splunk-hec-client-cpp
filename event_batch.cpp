//
// Created by kchen on 12/3/17.
//

#include "event_batch.h"

using namespace std;
using namespace rapidjson;

namespace splunkhec {

vector<unsigned char> EventBatch::serialize() const {
    vector<unsigned char> bytes;
    StringBuffer buffer;
    for (const auto& ev: events_) {
        ev->serialize(buffer);
        size_t size = bytes.size();
        bytes.resize(bytes.size() + buffer.GetSize() + line_breaker_.size());
        memcpy(&bytes[size], buffer.GetString(), buffer.GetSize());
        memcpy(&bytes[size + buffer.GetSize()], line_breaker_.data(), line_breaker_.size());

        buffer.Clear();
    }
    return bytes;
}

void EventBatch::add(EventInf* event) {
    assert (event != nullptr);
    events_.emplace_back(event);
}

EventBatch& EventBatch::add_extra_fields(const map<string, string>& fields) {
    for (auto& event: events_) {
        event->add_fields(fields);
    }
    return *this;
}

} // namespace splunkhec

