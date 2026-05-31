#pragma once
#include <cstddef>
#include <string>
#include <format>

class LazySequenceException : public std::exception {
private:
    std::string message_;
public:
    explicit LazySequenceException(const std::string& message) : message_(message) {}
    
    const char* what() const noexcept override {
        return message_.c_str();
    }
};

class LazySequenceOutOfRange : public LazySequenceException {
public:
    LazySequenceOutOfRange(size_t index) 
        : LazySequenceException(std::format("index {} is out of range in LazySequence", index)) {};
};

class LazySequenceIsInfinite : public LazySequenceException {
public:
    LazySequenceIsInfinite() 
        : LazySequenceException(std::format("LazySequence is infinite")) {};
};

class LazySequenceIsEmpthy : public LazySequenceException {
public:
    LazySequenceIsEmpthy() 
        : LazySequenceException(std::format("LazySequence is empthy")) {};
};

class LazySequenceGetSubsequence : public LazySequenceException {
public:
    LazySequenceGetSubsequence(size_t index1, size_t index2) 
        : LazySequenceException(std::format("index {} < index {}", index1, index2)) {};
};
