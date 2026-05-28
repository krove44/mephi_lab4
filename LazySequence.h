#pragma once
#include "mephi_lab2/Sequence/Sequence.h"
#include "mephi_lab2/Sequence/ListSequence.h"
#include <cstddef>
#include <functional>
#include <initializer_list>
#include "Generator.h"
#include "Exception/LazySequenceException.h"



template <template <typename> class Container, typename T>
class LazySequence {
private:
    Container<T> cache_;
    std::shared_ptr<Generator<T>> generator_;
    
public:

    LazySequence() : generator_(nullptr){};

    LazySequence(const Container<T>& data) : generator_(nullptr), cache_(data){};

    LazySequence(std::initializer_list<T> data) : generator_(nullptr), cache_(data){};

    T GetFirst() const {
        return cache_[0];
    };

    T GetLast() const {
        //пупупу
    };

    T Get(size_t index) const {
        if (index < cache_.GetLenght()){
            return cache_[index];
        }
        throw LazySequenceOutOfRange(index);
    }

    size_t GetLenght() const {
        //потом реализую для бесконечных
        return cache_.GetLenght();
    }

    LazySequence<Container, T>* GetSubsequence(size_t startIndex, size_t endIndex) const {return &LazySequence();};

    LazySequence<Container, T>* Append(T item) {return this;};

    LazySequence<Container, T>* Prepend(T item) {return this;};

    LazySequence<Container, T>* InsertAt(T item, size_t index) {return this;};
    
    LazySequence<Container, T>*  Concat(const ISequence<T>* list) {return this;};

};