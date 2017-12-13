//
// Created by kchen on 12/3/17.
//

#ifndef SPLUNK_HEC_CLIENT_CPP_HEC_EXCEPTION_H
#define SPLUNK_HEC_CLIENT_CPP_HEC_EXCEPTION_H

#include <exception>
#include <string>

namespace splunkhec {

class HecException: public std::exception {
public:
    HecException(const std::string& msg, int code);
    const char* what() const noexcept override;

    int error_code() const {
        return code_;
    }

private:
    std::string msg_;
    int code_;
};

} // namespace splunkhec

#endif //SPLUNK_HEC_CLIENT_CPP_HEC_EXCEPTION_H
