//
// Created by kchen on 11/29/17.
//

#include <map>

#include "../json_event.h"
#include "../rapidjson/include/rapidjson/prettywriter.h"


using namespace std;
using namespace rapidjson;

template <typename T>
static void serialize(T t) {
    auto event = new JsonEvent<T>(t, nullptr);
    event->setTime(1000);
    event->setHost("localhost");
    event->setSource("source");
    event->setSourcetype("sourcetype");
    event->setIndex("index");
    map<string, string> fields{{"k1", "v1"}, {"k2", "v2"}};
    event->setFields(fields);
    StringBuffer sb;
    PrettyWriter<StringBuffer> writer(sb);
    event->serialize(writer);
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
            ::serialize(writer, i);
        }
        writer.EndArray();

        return writer;
    }
};


int __main(int argc, const char** argv) {
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