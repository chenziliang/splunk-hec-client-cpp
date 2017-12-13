//
// Created by kchen on 12/3/17.
//

#ifndef SPLUNK_HEC_CLIENT_CPP_INDEXER_INF_H
#define SPLUNK_HEC_CLIENT_CPP_INDEXER_INF_H

#include "event_batch.h"

#include <memory>
#include <string>
#include <vector>

namespace splunkhec {

class IndexerInf {
public:
    virtual ~IndexerInf() {}
    virtual void send(const std::shared_ptr<EventBatch>& batch) const = 0;
    virtual std::string post(const std::string& uri, const std::vector<unsigned char>& data, const std::string& content_type) const = 0;
    virtual bool has_backpressure() const = 0;
    virtual std::string channel() const = 0;
};

} // splunkhec

#endif //SPLUNK_HEC_CLIENT_CPP_INDEXER_INF_H
