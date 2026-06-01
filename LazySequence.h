#pragma once
#include <cstddef>
#include <initializer_list>
#include <memory>
#include "Generators\RecurentGenerator.h"
#include "Generators\IGenerator.h"
#include "Exception/LazySequenceException.h"
#include "Cardinal.h"


template <template <typename> class Container, typename T>
class LazySequence {
private:
    Container<T> cache_;
    Container<T> suffix_;
    size_t generator_offset_ = 0;
    std::shared_ptr<IGenerator<Container, T>> generator_;

    size_t FiniteSize() const {
        size_t size = cache_.GetLenght();
        if (generator_) {
            Cardinal gen_size = generator_->Size();
            if (gen_size.IsFinite()) {
                size += gen_size.Value();
            }
        }
        return size;
    }
    
public:

    LazySequence() : generator_(nullptr), cache_{}{};

    LazySequence(const Container<T>& data) : generator_(nullptr), cache_(data){};

    LazySequence(const LazySequence<Container, T>& other) : cache_(other.cache_), suffix_(other.suffix_), generator_(other.generator_){};

    LazySequence(std::initializer_list<T> data) : generator_(nullptr), cache_(data){};
    
    LazySequence(std::shared_ptr<IGenerator<Container, T>> generator) : generator_(generator), cache_{}{}

    T GetFirst() const {
        return Get((size_t)0);
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
            size_t gen_ = index - cache_.GetLenght() + generator_offset_;
            return generator_->Get(Cardinal(gen_));
        }
        throw LazySequenceOutOfRange(index);
    }

    Cardinal GetLenght() const {
        if (IsInfinite()) {
            return Cardinal::Omega();
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

    LazySequence<Container, T> Append(T item) {
        LazySequence<Container, T> result(*this);
        if (IsInfinite()) {
            result.suffix_.Append(item);
        } 
        else {
            result.cache_.Append(item);
        }
        return result;
    };

    LazySequence<Container, T> Prepend(T item) {
        LazySequence<Container, T> result(*this);
        result.cache_.Prepend(item);
        return result;
    };

    LazySequence<Container, T> InsertAt(T item, size_t index) {
        if (!IsInfinite() && index > FiniteSize()) {
            throw LazySequenceOutOfRange(index);
        }
        LazySequence<Container, T> result(*this);
        if (!IsInfinite()) {
            result.cache_.InsertAt(item, index);
            return result;
        }
        for(size_t i = result.cache_.GetLenght(); i < index; ++i){
            result.cache_.Append(Get(i));
        }
        result.cache_.Append(item);
        result.generator_offset_ = cache_.GetLenght();
        return result;
    };
    
    LazySequence<Container, T> Concat(const LazySequence<Container, T>& other) const;

    bool IsInfinite() const {
        return generator_ && generator_->Size().IsInfinite();
    }

    T GetByCardinal(Cardinal index) const {
        if (index.IsFinite()) {
            return Get(index.Value());
        }
        if (generator_) {
            return generator_->Get(index);
        }
        throw LazySequenceException("bad index");
    }
};

#include "Generators\ConcatGenerator.h"
#include "Generators\Sequencegenerator.h"


template <template <typename> class Container, typename T>
LazySequence<Container, T> LazySequence<Container, T>::Concat(const LazySequence<Container, T>& other) const {
    auto gen_self  = std::make_shared<SequenceGenerator<Container, T>>(*this);
    auto gen_other = std::make_shared<SequenceGenerator<Container, T>>(other);
    auto gen_concat = std::make_shared<ConcatGenerator<Container, T>>(gen_self, gen_other);
    return LazySequence<Container, T>(std::static_pointer_cast<IGenerator<Container, T>>(gen_concat));
}
