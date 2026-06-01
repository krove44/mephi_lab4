#pragma once
#include <cstddef>
#include <string>
#include <format>

class SequenceGeneratorException : public std::exception {
private:
    std::string message_;
public:
    explicit SequenceGeneratorException(const std::string& message) : message_(message) {}
    
    const char* what() const noexcept override {
        return message_.c_str();
    }
};

