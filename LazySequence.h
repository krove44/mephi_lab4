#pragma once
#include "mephi_lab2/Sequence/Sequence.h"
#include "mephi_lab2/Sequence/ListSequence.h"
#include <cstddef>
#include <functional>

template<typename T>
class LazySequence{
private:
    ListSequence<T> cache_;
    std::function<T(ListSequence<T>*)> rule_;
    size_t arity_;
    
public:
    LazySequence(const ListSequence<T>& data, std::function<T(ListSequence<T>*)> rule, size_t arity) : cache_(data),  rule_(rule), arity_(arity){};

};