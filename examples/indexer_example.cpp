//
// Created by kchen on 12/3/17.
//


#include <iostream>
#include <vector>
#include <json_event.h>

#include "indexer.h"
#include "config.h"
#include "poller_inf.h"
#include "hec_channel.h"
#include "json_event_batch.h"

using namespace std;
using namespace splunkhec;

class DummyPoller: public PollerInf {
public:
    void start() override {};
    void stop() override {};
    void add(const HecChannel& channel, const EventBatch& batch, const std::string& response) override {
        cout << response << endl;
    }
    void fail(const HecChannel& channel, const EventBatch& batch, const std::exception& ex) override {}
};

using namespace std;
using namespace splunkhec;

int main(int argc, const char** argv) {
    Config config;
    config.http_validate_certs_ = false;
    DummyPoller poller;

    Indexer idx("https://localhost:8088", "1B901D2B-576D-40CD-AF1E-98141B499534", poller, config);
    JsonEventBatch batch;
    batch.add(new JsonEvent<const char*>("hello, rest cpp sdk"));
    idx.send(batch);

    return 0;
}