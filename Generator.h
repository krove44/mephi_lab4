#pragma once
#include <cstddef>
#include <functional>
#include "Exception/GeneratorException.h"

template <template <typename> class Container, typename T>
class Generator {
private:
    std::function<T(const Container<T>&)> rule_;
    Container<T> rule_cache_;
    bool is_finite_;
    size_t finite_size_;
public:
    Generator(std::function<T(const Container<T>&)> rule, Container<T> val) : rule_(std::move(rule)), 
        rule_cache_(std::move(val)), is_finite_(false), finite_size_(0){};

    Generator(std::function<T(const Container<T>&)> rule, std::vector<T> val, size_t size) : rule_(std::move(rule)), 
        rule_cache_(std::move(val)), is_finite_(true), finite_size_(size){};

    const T& Materialize(size_t index) {
        if (is_finite_ && index >= finite_size_) {
            throw GeneratorOutOfRange(index);
        }
        while (rule_cache_.size() <= index) {
            rule_cache_.push_back(rule_(rule_cache_));
        }
        return rule_cache_[index];
    }

    

    
};