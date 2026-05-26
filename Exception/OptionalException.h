#pragma once
#include <string>
#include <format>

class OptionalException : public std::exception {
private:
    std::string message_;
public:
    explicit OptionalException(const std::string& message) : message_(message) {}
    
    const char* what() const noexcept override {
        return message_.c_str();
    }
};

class OptionalEmpthy : public OptionalException {
public:
    OptionalEmpthy(const std::string& msg) 
        : OptionalException(msg) {}
};
