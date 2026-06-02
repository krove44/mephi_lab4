#pragma once
#include <cstddef>
#include <functional>
#include "../Exception/RecurentGeneratorException.h"
#include "IGenerator.h"
#include <memory>

template <template <typename> class Container, typename T>
class RecurentGenerator : public IGenerator<Container, T>{
private:
    std::function<T(const Container<T>&)> rule_;
    Container<T> rule_cache_;
    bool is_finite_;
    size_t finite_size_;
    size_t cursor_ = 0;
    size_t offset_ = 0;

    void Materialize(size_t index) {
        if (is_finite_ && index >= finite_size_) {
            throw RecurentGeneratorOutOfRange(index);
        }
        while (rule_cache_.GetLenght() <= index) {
            rule_cache_.Append(rule_(rule_cache_));
        }
    }


public:
    RecurentGenerator(std::function<T(const Container<T>&)> rule, Container<T> val) : rule_(std::move(rule)), 
        rule_cache_(std::move(val)), is_finite_(false), finite_size_(0){};

    RecurentGenerator(std::function<T(const Container<T>&)> rule, Container<T> val, size_t size) : rule_(std::move(rule)), 
        rule_cache_(std::move(val)), is_finite_(true), finite_size_(size){};

    bool HasNext() const override {
        if (is_finite_) {
            return cursor_ < finite_size_;
        }
        return true;
    }
    //TODO:: использовать optional
    T GetNext() override {
        if (!HasNext()) {
            throw RecurentGeneratorException("HasNext is false");
        }
        Materialize(cursor_);
        return rule_cache_[cursor_++];
    }

    T Current() const override {
        if (cursor_ == 0) {
            throw RecurentGeneratorException("Current nothing");
        }
        return rule_cache_[cursor_ - 1];
    }

    T Get(Cardinal index) override {
        if (index.IsInfinite()) {
            throw RecurentGeneratorException("transfinite index");
        }
        size_t i = index.Value() + offset_;
        Materialize(i);
        return rule_cache_[i];
    }

    Cardinal Size() const override {
        if (!is_finite_) {
            return Cardinal::Omega();
        }
        return Cardinal(finite_size_);
    }

    std::shared_ptr<IGenerator<Container, T>> Slice(Cardinal offset) const override {
        if (offset.IsInfinite()) {
            throw RecurentGeneratorException("Slice with no finite offset");
        }
        size_t off = offset.Value();
        const_cast<RecurentGenerator*>(this)->Materialize(off > 0 ? off - 1 : 0);
 
        size_t new_size = is_finite_ ? (finite_size_ > off ? finite_size_ - off : 0) : 0;
        std::shared_ptr<RecurentGenerator<Container, T>> sliced;
        if (is_finite_) {
            sliced = std::make_shared<RecurentGenerator<Container, T>>(rule_, rule_cache_, new_size);
        }
        else {
            sliced = std::make_shared<RecurentGenerator<Container, T>>(rule_, rule_cache_);
        }
        sliced->cursor_ = off;
        sliced->offset_ = off;
        return sliced;
    }

    bool IsFinite() const {
        return is_finite_;
    }

    size_t FiniteSize() const {
        return finite_size_;
    }
    
    const T& MaterializeAndReturn(size_t index) {
        Materialize(index);
        return rule_cache_[index];
    }
};