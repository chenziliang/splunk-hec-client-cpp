//
// Created by kchen on 12/3/17.
//

#include "json_event.h"
#include "poller_callback_inf.h"

#include "indexer.h"
#include "poller_inf.h"
#include "json_event_batch.h"
#include "http_response_poller.h"
#include "http_client_factory.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace splunkhec;

class PollerCallback: public PollerCallbackInf {
public:
    void on_event_failed(const std::vector<std::shared_ptr<EventBatch>>& failure, const std::exception& ex) override {
        cout << "failed: " << ex.what() << endl;
    }

    void on_event_committed(const std::vector<std::shared_ptr<EventBatch>>& committed) override {
        cout << "committed: " << committed.size() << endl;
    }
};

using namespace std;
using namespace splunkhec;

int main(int argc, const char** argv) {
    shared_ptr<PollerCallbackInf> callback{new PollerCallback()};
    shared_ptr<HttpResponsePoller> poller{new HttpResponsePoller(callback)};

    HttpClientFactory factory;
    factory.set_validate_certificates(false);

    Indexer idx("https://localhost:8088", "1B901D2B-576D-40CD-AF1E-98141B499534", poller, factory);
    shared_ptr<EventBatch> batch(new JsonEventBatch);
    batch->add(new JsonEvent<const char*>("hello, rest cpp sdk"));
    idx.send(batch);

    return 0;
}