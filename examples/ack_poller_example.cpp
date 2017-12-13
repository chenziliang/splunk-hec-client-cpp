//
// Created by kchen on 12/12/17.
//

#include "ack_poller.h"
#include "raw_event_batch.h"
#include "http_client_factory.h"
#include "raw_event.h"
#include "indexer.h"

#include <iostream>
#include <chrono>

using namespace splunkhec;
using namespace std;

class DummyCallback: public PollerCallbackInf {
public:
    void on_event_failed(const vector<shared_ptr<EventBatch>>& failure, const exception& ex) override {
        cout << "failed event batch: " << ex.what() << endl;
    }
    void on_event_committed(const vector<shared_ptr<EventBatch>>& committed) override {
        cout << "committed event batch" << endl;
    }
};

shared_ptr<EventBatch> create_batch() {
    auto batch = RawEventBatch::Builder().build();
    batch->add(new RawEvent<string>("test ack poller 1", nullptr));
    batch->add(new RawEvent<string>("test ack poller 2", nullptr));
    batch->add(new RawEvent<string>("test ack poller 3", nullptr));

    return shared_ptr<EventBatch> (batch);
}

int main(int argc, char** argv) {
    string uri{"https://localhost:8088"};
    string token{"00000000-0000-0000-0000-000000000001"};
    HttpClientFactory factory;
    factory.set_validate_certificates(false);

    shared_ptr<PollerCallbackInf> callback = make_shared<DummyCallback>();
    shared_ptr<AckPoller> poller{new AckPoller(2, callback)};
    poller->set_ack_poll_interval(chrono::seconds{2});
    poller->set_event_batch_ttl(chrono::seconds{30});

    shared_ptr<IndexerInf> indexer(new Indexer(uri, token, poller, factory));

    cout << "Going to start" << endl;
    string response = "{\"text\":\"Success\",\"code\":0, \"ackId\":7}";
    string response2 = "{\"text\":\"Success\",\"code\":0, \"ackId\":8}";
    string response3 = "{\"text\":\"Success\",\"code\":0, \"ackId\":9}";

    poller->start();
    poller->add(indexer, create_batch(), response);
    poller->add(indexer, create_batch(), response2);
    poller->add(indexer, create_batch(), response3);

    cout << "Going to sleep" << endl;
    this_thread::sleep_for(chrono::seconds{1200});
    poller->stop();
    cout << "Done !" << endl;

    return 0;
}