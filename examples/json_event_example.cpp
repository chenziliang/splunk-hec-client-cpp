//
// Created by kchen on 11/29/17.
//

#include "json_event.h"

#include <map>


using namespace std;
using namespace rapidjson;
using namespace splunkhec;

template <typename T>
static void serialize(T t) {
    auto event = new JsonEvent<T>(t, nullptr);
    event->set_time(1000);
    event->set_host("localhost");
    event->set_source("source");
    event->set_sourcetype("sourcetype");
    event->set_index("index");
    map<string, string> fields{{"k1", "v1"}, {"k2", "v2"}};
    event->set_fields(fields);
    StringBuffer sb;
    event->serialize(sb);
    puts(sb.GetString());
}

class WrapArray {
private:
    int a[3] = {1, 2, 3};

public:
    template <typename Writer>
    Writer& serialize(Writer& writer) const {
        writer.StartArray();
        for (auto i: a) {
            splunkhec::serialize(writer, i);
        }
        writer.EndArray();

        return writer;
    }
};


int main(int argc, const char** argv) {
    serialize("hello world");
    serialize(string("hello world string"));
    serialize(100);
    serialize(unsigned(100));
    serialize(int64_t(100));
    serialize(uint64_t(1000));
    serialize(18.0);
    serialize(true);
    serialize(nullptr);
    serialize(WrapArray());

    return 0;
}