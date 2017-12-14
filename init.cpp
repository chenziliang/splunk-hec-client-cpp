//
// Created by kchen on 12/13/17.
//

#include "init.h"

#include <spdlog/spdlog.h>

namespace splunkhec {

void init_default() {
    spdlog::stderr_logger_mt("splunk-hec");
    spdlog::set_pattern("[%H:%M:%S %z] [thread %t] %v");
}

} // namespace splunkhec

namespace // anonymous
{
    struct Init {
        Init() {
            splunkhec::init_default();
        }
    };
    Init init;
}
