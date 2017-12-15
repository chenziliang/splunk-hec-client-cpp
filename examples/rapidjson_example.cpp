//
// Created by kchen on 12/12/17.
//

#include "hec_response_parser.h"

#include <vector>
#include <iostream>
#include <string>
#include <cassert>
#include <iterator>

using namespace std;
using namespace splunkhec;

int main(int argc, const char** argv) {
    string response = "{\"text\":\"Success\",\"code\":0}";
    int code = parse_response(response);
    assert (code == 0);
    cout << "code: " << code << endl;

    response = "{\"text\":\"Success\",\"code\":9}";
    code = parse_response(response);
    assert (code == 9);
    cout << "code: " << code << endl;

    response = "{\"text\":\"Success\",\"code\":0, \"ackId\":7}";
    auto code_ackid = parse_response_with_ackid(response);
    assert (code_ackid.first == 0);
    assert (code_ackid.second == 7);
    cout << "code: " << code_ackid.first << " ackid: " << code_ackid.second << endl;

    response = "{\"acks\":{\"8\":false,\"9\":false,\"10\":true}}";
    vector<int64_t> committed_ids;
    bool ok = parse_ack_poll_response(response, committed_ids);
    assert(ok);
    assert(committed_ids.size() == 1);
    assert(committed_ids[0] == 10);

    vector<int64_t> acks{8, 9, 10};
    vector<unsigned char> bytes = prepare_ack_poll_payload(acks);
    copy(bytes.begin(), bytes.end(), ostream_iterator<unsigned char>(cout, ""));

    return 0;
}

