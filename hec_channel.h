//
// Created by kchen on 12/3/17.
//

#ifndef SPLUNK_HEC_CLIENT_CPP_HEC_CHANNEL_H
#define SPLUNK_HEC_CLIENT_CPP_HEC_CHANNEL_H

#include <utility>

#include <string>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include "indexer_inf.h"
#include "event_batch.h"

namespace splunkhec {

class HecChannel {
public:
    explicit HecChannel(IndexerInf& indexer)
            : indexer_(indexer) {
        auto uuid{boost::uuids::random_generator()()};
        std::string id{boost::uuids::to_string(uuid)};
        std::swap(uuid_, id);
    }

    const IndexerInf& indexer() const {
        return indexer_;
    }

    IndexerInf& indexer() {
        return indexer_;
    }

    void send(const EventBatch& batch) {
        indexer_.send(batch);
    }

    const std::string& id() const {
        return uuid_;
    }

private:
    IndexerInf& indexer_;
    std::string uuid_;
};

} // namespace splunkhec

#endif //SPLUNK_HEC_CLIENT_CPP_HEC_CHANNEL_H
