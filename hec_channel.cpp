//
// Created by kchen on 12/3/17.
//

#include "hec_channel.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include <utility>

using namespace std;

namespace splunkhec {

HecChannel::HecChannel(IndexerInf& indexer) : indexer_(indexer) {
    auto uuid{boost::uuids::random_generator()()};
    string id{boost::uuids::to_string(uuid)};
    swap(uuid_, id);
}

} // namespace splunkhec

