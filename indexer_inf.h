//
// Created by kchen on 12/3/17.
//

#ifndef SPLUNK_HEC_CLIENT_CPP_INDEXER_INF_H
#define SPLUNK_HEC_CLIENT_CPP_INDEXER_INF_H

#include "event_batch.h"

namespace splunkhec {

class IndexerInf {
public:
    virtual ~IndexerInf() {}
    virtual void send(const EventBatch& batch) const = 0;
    virtual bool has_backpressure() const = 0;
};

} // splunkhec

#endif //SPLUNK_HEC_CLIENT_CPP_INDEXER_INF_H
