//
// Created by kchen on 12/3/17.
//

#include "hec_exception.h"

using namespace std;

namespace splunkhec {

HecException::HecException(const string& msg): msg_(msg){
}

const char* HecException::what() const noexcept {
    return msg_.data();
}

} // namespace splunkhec

