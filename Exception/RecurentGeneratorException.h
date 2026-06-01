#pragma once
#include <cstddef>
#include <string>
#include <format>

class RecurentGeneratorException : public std::exception {
private:
    std::string message_;
public:
    explicit RecurentGeneratorException(const std::string& message) : message_(message) {}
    
    const char* what() const noexcept override {
        return message_.c_str();
    }
};

class RecurentGeneratorOutOfRange : public RecurentGeneratorException {
public:
    RecurentGeneratorOutOfRange(size_t index) 
        : RecurentGeneratorException(std::format("index {} is out of range in RecurentGenerator", index)) {};
};
