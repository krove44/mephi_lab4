#pragma once
#include <memory>
#include "IGenerator.h"
#include "../Cardinal.h"
#include "../Exception/ConcatGenerateException.h"

template <template <typename> class Container, typename T>
class ConcatGenerator : public IGenerator<Container, T> {
private:
    std::shared_ptr<IGenerator<Container, T>> first_;
    std::shared_ptr<IGenerator<Container, T>> second_;
    bool on_second_ = false;

public:
    ConcatGenerator(std::shared_ptr<IGenerator<Container, T>> first, std::shared_ptr<IGenerator<Container, T>> second): 
        first_(std::move(first)), second_(std::move(second)){}

    bool HasNext() const override {
        if (!on_second_) {
            return first_->HasNext() || second_->HasNext();
        }
        return second_->HasNext();
    }

    T GetNext() override {
        if (!HasNext()) {
            throw ConcatGeneratorException("no elements");
        }
        if (!on_second_) {
            if (first_->HasNext()) {
                return first_->GetNext();
            }
            on_second_ = true;
        }
        return second_->GetNext();
    }

    T Current() const override {
        if (on_second_) {
            return second_->Current();
        }
        return first_->Current();
    }

    T Get(Cardinal index) override {
        Cardinal first_size = first_->Size();
        if (first_size.IsInfinite()) {
            if (index.IsTransfinite()) {
                return second_->Get(Cardinal(index.FinitePart()));
            }
            if (index.IsFinite()) {
                return first_->Get(index);
            }
            throw ConcatGeneratorException("bad index");
        }

        if (index < first_size) {
            return first_->Get(index);
        }
        return second_->Get(index - first_size);
    }

    Cardinal Size() const override {
        return first_->Size() + second_->Size();
    }

    std::shared_ptr<IGenerator<Container, T>> Slice(Cardinal offset) const override {
        Cardinal first_size = first_->Size();
        if (first_size.IsInfinite()) {
            if (offset.IsFinite()) {
                auto sliced_first = first_->Slice(offset);
                return std::make_shared<ConcatGenerator<Container, T>>(sliced_first, second_);
            }
            if (offset.IsTransfinite()) {
                return second_->Slice(Cardinal(offset.FinitePart()));
            }
            throw ConcatGeneratorException("index is infinite");
        }
        if (offset < first_size) {
            auto sliced_first = first_->Slice(offset);
            return std::make_shared<ConcatGenerator<Container, T>>(sliced_first, second_);
        }
        return second_->Slice(offset - first_size);
    }
};