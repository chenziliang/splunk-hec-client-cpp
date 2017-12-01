//
// Created by kchen on 11/30/17.
//

#include "json_event_batch.h"

using namespace std;

namespace splunkhec {

string JsonEventBatch::rest_endpoint() const {
    return event_endpoint;
}

string JsonEventBatch::content_type() const {
    return event_content_type;
}

JsonEventBatch* JsonEventBatch::create_from_this() const {
    return new JsonEventBatch();
}

const string JsonEventBatch::event_endpoint = "/services/collector/event";
const string JsonEventBatch::event_content_type = "application/json; profile=urn:splunk:event:1.0; charset=utf-8";

} // namespace splunkhec
