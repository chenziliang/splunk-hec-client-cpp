//
// Created by kchen on 12/13/17.
//

#include "concurrent_hec.h"
#include "json_event_batch.h"
#include "json_event.h"
#include "config.h"

#include <iostream>

using namespace splunkhec;
using namespace std;

class PollerCallback: public PollerCallbackInf {
public:
    void on_event_failed(const std::vector<std::shared_ptr<EventBatch>>& failure, const std::exception& ex) override {
        cout << "failed: " << ex.what() << endl;
    }

    void on_event_committed(const std::vector<std::shared_ptr<EventBatch>>& committed) override {
        cout << "committed: " << committed.size() << endl;
    }
};

shared_ptr<EventBatch> new_batch(int batch_size) {
    auto batch = make_shared<JsonEventBatch>();
    for (int i = 0; i < batch_size; ++i) {
        batch->add(new JsonEvent<const char*>("12-13-2017 14:26:49.007 -0800 INFO  Metrics - group=queue, name=aeq, max_size_kb=500, current_size_kb=0, current_size=0, largest_size=0, smallest_size=0"));
    }
    return batch;
}

int main(int argc, char** argv) {
    vector<string> uris{"https://localhost:8088"};
    string token{"00000000-0000-0000-0000-000000000001"};
    Config config(uris, token);
    config.total_channels_ = 4;
    config.http_validate_certs_ = false;

    auto callback = make_shared<PollerCallback>();

    ConcurrentHec hec(4, true, config, callback);

    for (int i = 0; i < 1000000; ++i) {
        while (1) {
            auto batch = new_batch(500);
            bool ok = hec.send(batch);
            if (ok) {
                break;
            }
        }
    }

    hec.stop();

    return 0;
}
