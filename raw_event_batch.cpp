//
// Created by kchen on 11/30/17.
//

#include "raw_event_batch.h"

using namespace std;

namespace splunkhec {

RawEventBatch::RawEventBatch(const string& index, const string& source,
                             const string& sourcetype, const string& host, int64_t time) :
        index_(index), source_(source), sourcetype_(sourcetype), host_(host), time_(time) {
}


string RawEventBatch::content_type() const {
    return raw_content_type;
}

RawEventBatch* RawEventBatch::create_from_this() const {
    return Builder().set_index(index_)
        .set_source(source_)
        .set_sourcetype(sourcetype_)
        .set_host(host_)
        .set_time(time_)
        .build();
}

string RawEventBatch::rest_endpoint() const {
    return raw_endpoint + metadata_params();
}

static void put_if_present(vector<string>& metas, const string& key, const string& value) {
    if (!key.empty()) {
        metas.push_back(key + "=" + value);
    }
}

string RawEventBatch::metadata_params() const {
    // FIXME uri builder (escaping)
    vector<string> metas;
    put_if_present(metas, sIndex, index_);
    put_if_present(metas, sSource, source_);
    put_if_present(metas, sSourcetype, sourcetype_);
    put_if_present(metas, sHost, host_);

    if (time_ > 0) {
        put_if_present(metas, sTime, to_string(time_));
    }

    if (metas.empty()) {
        return "";
    }

    string params{"?"};
    for (const auto& p: metas) {
        params.append(p);
        params.append("&");
    }
    params.resize(params.size() - 1);
    return params;
}

const string RawEventBatch::raw_endpoint = "/services/collector/raw";
const string RawEventBatch::raw_content_type = "text/plain; profile=urn:splunk:event:1.0; charset=utf-8";


} // namespace splunkhec
