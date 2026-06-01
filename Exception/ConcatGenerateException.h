#pragma once
#include <cstddef>
#include <string>
#include <format>


class ConcatGeneratorException : public std::exception {
private:
    std::string message_;
public:
    explicit ConcatGeneratorException(const std::string& message) : message_(message) {}
    
    const char* what() const noexcept override {
        return message_.c_str();
    }
};