#pragma once
#include <cstddef>
#include <string>
#include <format>

class GeneratorException : public std::exception {
private:
    std::string message_;
public:
    explicit GeneratorException(const std::string& message) : message_(message) {}
    
    const char* what() const noexcept override {
        return message_.c_str();
    }
};

class GeneratorOutOfRange : public GeneratorException {
public:
    GeneratorOutOfRange(size_t index) 
        : GeneratorException(std::format("index {} is out of range in Generator", index)) {};
};
