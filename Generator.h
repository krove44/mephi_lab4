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

    Generator(std::function<T(const Container<T>&)> rule, Container<T> val, size_t size) : rule_(std::move(rule)), 
        rule_cache_(std::move(val)), is_finite_(true), finite_size_(size){};

    const T& Materialize(size_t index) {
        if (is_finite_ && index >= finite_size_) {
            throw GeneratorOutOfRange(index);
        }
        while (rule_cache_.GetLenght() <= index) {
            rule_cache_.Append(rule_(rule_cache_));
        }
        return rule_cache_[index];
    }



    bool IsFinite() const {
        return is_finite_;
    }

    size_t FiniteSize() const {
        return finite_size_;
    }
    

    
};