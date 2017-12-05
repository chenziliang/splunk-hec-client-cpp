//
// Created by kchen on 12/3/17.
//

#include "ack_poller.h"

namespace splunkhec {

void AckPoller::start() {
}

void AckPoller::stop() {
}

void AckPoller::add(const HecChannel& channel, const std::shared_ptr<EventBatch>& batch, const std::string& response) {
}

void AckPoller::fail(const HecChannel& channel, const std::shared_ptr<EventBatch>& batch, const std::exception& ex) {
}

} // namespace splunkhec
