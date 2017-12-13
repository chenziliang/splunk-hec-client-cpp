//
// Created by kchen on 12/12/17.
//

#include "load_balancer.h"

using namespace std;

namespace splunkhec {

void LoadBalancer::add(const shared_ptr<IndexerInf>& indexer) {
    indexers_.push_back(indexer);
}

void LoadBalancer::remove(const shared_ptr<IndexerInf>& indexer) {
    auto end = indexers_.end();
    for (auto it = indexers_.begin(); it != end; ++it) {
        if ((*it)->channel() == indexer->channel()) {
            indexers_.erase(it);
            break;
        }
    }
}

// return false when failed to send
bool LoadBalancer::send(const shared_ptr<EventBatch>& batch) {
    if (indexers_.empty()) {
        return false;
    }

    for (int retried = 0; retried != indexers_.size(); ++retried) {
        auto& indexer = indexers_[index_];
        index_ = (index_ + 1) % indexers_.size();
        if (!indexer->has_backpressure()) {
            indexer->send(batch);
            return true;
        }
    }

    // all indexers have backpressure
    return false;
}

size_t LoadBalancer::size() const {
    return indexers_.size();
}

} // namespace splunkhec