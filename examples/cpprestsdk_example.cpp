//
// Created by kchen on 12/1/17.
//


#include "http_client_factory.h"

#include <cpprest/http_client.h>
#include <iostream>
#include <memory>

using namespace std;

using namespace pplx;                    // Common utilities like string conversions
using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace web::json;
using namespace splunkhec;

int main(int argc, const char** argv) {
    HttpClientFactory factory;
    factory.set_validate_certificates(false);

    unique_ptr<http_client> client(factory.create("https://localhost:8088/services/collector/event"));

    http_request req(methods::POST);
    req.headers().set_content_type("application/json");
    req.headers().add("X-Splunk-Request-Channel", "FE0ECFAD-13D5-401B-847D-77833BD77131");
    req.headers().add("Authorization", "Splunk 1B901D2B-576D-40CD-AF1E-98141B499534");

    value v;
    v["index"] = value::string("main");
    v["sourcetype"] = value::string("cpprestsdk");
    v["source"] = value::string("cpprestsdk");
    v["event"] = value::object();
    v["event"]["k1"] = value::string("v1");
    v["event"]["k2"] = value::string("v2");
    req.set_body(v);

    task<void> res = client->request(req).then([=](task<http_response> t) {
        try {
            auto response = t.get();
            // cout << response.extract_string().get() << endl;
            auto r = response.extract_json().get();
            cout << r["text"] << " " << r["code"] << " " << r["ackId"] << endl;

        } catch (const http_exception& ex) {
            cout << ex.what() << endl;
        }

    });
    res.get();

    return 0;
}

