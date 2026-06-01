#pragma once
#include <cstddef>
#include <string>
#include <format>


class CardinalException : public std::exception {
private:
    std::string message_;
public:
    explicit CardinalException(const std::string& message) : message_(message) {}
    
    const char* what() const noexcept override {
        return message_.c_str();
    }
};