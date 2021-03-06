//
// Created by kchen on 11/30/17.
//

#include <iostream>
#include <string>
#include <vector>

#include "raw_event_batch.h"
#include "raw_event.h"

using namespace std;
using namespace splunkhec;

int main(int argc, const char** argv) {
    RawEventBatch::Builder builder;
    auto batch = builder.set_index("main")
            .set_source("source")
            .set_sourcetype("sourcetype")
            .set_host("host")
            .set_time(100)
            .build();

    auto re = new RawEvent<const char*>("hello", nullptr);
    batch->add(re);
    auto re2 = new RawEvent<const char*>("world", nullptr);
    batch->add(re2);
    batch->set_linebreaker("\n");

    cout << batch->rest_endpoint() << endl;
    vector<unsigned char> data(batch->serialize());
    cout << string(data.data(), data.data() + data.size()) << endl;
    delete batch;
    return 0;
}

