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
    explicit HecException(const std::string& msg);
    const char* what() const noexcept override;

private:
    std::string msg_;
};

} // namespace splunkhec

#endif //SPLUNK_HEC_CLIENT_CPP_HEC_EXCEPTION_H
