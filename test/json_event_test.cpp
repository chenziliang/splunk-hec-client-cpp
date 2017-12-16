//
// Created by kchen on 12/15/17.
//

#include "catch.hpp"

#include "json_event.h"

#include <string>
#include <map>

using namespace splunkhec;
using namespace std;


TEST_CASE("json_event", "[JsonEvent]") {
    string hello{"hello"};
    string nihao{"ni hao"};
    auto event = JsonEvent<string>(hello, nihao);

    SECTION("fields") {
        auto& got = event.fields();
        REQUIRE(got.size() == 0);
    }

    SECTION("add_fields") {
        map<string, string> fields{{"ni", "hao"}};
        event.add_fields(fields);

        auto& got = event.fields();
        REQUIRE(got.size() == 1);
        REQUIRE(got.at("ni") == "hao");

        map<string, string> fields2{{"hello", "world"}};
        event.add_fields(fields2);

        auto& got2 = event.fields();
        REQUIRE(got2.size() == 2);
        REQUIRE(got2.at("hello") == "world");
    }

    SECTION("set_fields") {
        map<string, string> fields{{"ni", "hao"}};
        event.set_fields(fields);

        auto& got = event.fields();
        REQUIRE(got.size() == 1);
        REQUIRE(got.at("ni") == "hao");

        map<string, string> fields2{{"hello", "world"}};
        event.set_fields(fields2);

        auto& got2 = event.fields();
        REQUIRE(got2.size() == 1);
        REQUIRE(got2.at("hello") == "world");
    }

}
