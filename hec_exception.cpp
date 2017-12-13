//
// Created by kchen on 12/3/17.
//

#include "hec_exception.h"

using namespace std;

namespace splunkhec {

HecException::HecException(const string& msg, int code): msg_(msg), code_(code) {
}

const char* HecException::what() const noexcept {
    return msg_.c_str();
}

} // namespace splunkhec

