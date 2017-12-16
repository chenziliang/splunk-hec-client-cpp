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

    SECTION("add_fields") {
        map<string, string> fields{{"ni", "world"}};
        event.add_fields(fields);

        auto got = event.fields();
        REQUIRE(got.size() == 1);
        REQUIRE(got["ni"] == "world");
    }

}
