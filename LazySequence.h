#pragma once
#include "mephi_lab2/Sequence/Sequence.h"
#include "mephi_lab2/Sequence/ListSequence.h"
#include <cmath>
#include <cstddef>
#include <initializer_list>
#include "Generator.h"
#include "Exception/LazySequenceException.h"
#include "Cardinal.h"



template <template <typename> class Container, typename T>
class LazySequence {
private:
    Container<T> cache_;
    std::shared_ptr<Generator<Container, T>> generator_;

    size_t FiniteSize() const {
        size_t size = cache_.GetLenght();
        if (generator_ && generator_->IsFinite()) {
            size += generator_->FiniteSize();
        }
        return size;
    }
    
public:

    LazySequence() : generator_(nullptr), cache_{}{};

    LazySequence(const Container<T>& data) : generator_(nullptr), cache_(data){};

    LazySequence(std::initializer_list<T> data) : generator_(nullptr), cache_(data){};
    
    LazySequence(std::shared_ptr<Generator<Container, T>> generator) : generator_(generator), cache_{}{}

    T GetFirst() const {
        return Get(0);
    };

    T GetLast() const {
        if(IsInfinite()) {
            throw LazySequenceIsInfinite();
        }
        size_t len = FiniteSize();
        if (len == 0) {
            throw LazySequenceIsEmpthy();
        };
        return Get(len - 1);
    };

    T Get(size_t index) const {
        if (index < cache_.GetLenght()){
            return cache_[index];
        }
        if (generator_) {
            index -= cache_.GetLenght();
            return generator_->Materialize(index);
        }
        throw LazySequenceOutOfRange(index);
    }

    Cardinal GetLenght() const {
        if (IsInfinite()){
            return Cardinal(true);
        }
        return Cardinal(FiniteSize());
    }

    LazySequence<Container, T> GetSubsequence(size_t startIndex, size_t endIndex) const {
        if(startIndex > endIndex) {
            throw LazySequenceGetSubsequence(startIndex, endIndex);
        }
        Container<T> items;
        for (size_t i = startIndex; i <= endIndex; ++i) {
            items.Append(Get(i));
        }
        return LazySequence<Container, T>(items);
    };

    LazySequence<Container, T>* Append(T item) {return this;};

    LazySequence<Container, T>* Prepend(T item) {return this;};

    LazySequence<Container, T>* InsertAt(T item, size_t index) {return this;};
    
    LazySequence<Container, T>*  Concat(const ISequence<T>* list) {return this;};

    bool IsInfinite() const {
        return generator_ && !generator_->IsFinite();
    }
};