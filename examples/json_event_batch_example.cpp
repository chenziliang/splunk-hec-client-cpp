//
// Created by kchen on 11/30/17.
//


#include <iostream>
#include <vector>

#include "json_event_batch.h"
#include "json_event.h"

using namespace std;
using namespace splunkhec;

int main(int argc, const char** argv) {
    auto batch = new JsonEventBatch();
    auto re = new JsonEvent<const char*>("hello", nullptr);
    batch->add(re);
    auto re2 = new JsonEvent<const char*>("world", nullptr);
    batch->add(re2);

    cout << batch->rest_endpoint() << endl;
    vector<unsigned char> data(batch->serialize());
    copy(begin(data), end(data), ostream_iterator<unsigned char>(cout));

    delete batch;
    return 0;
}
