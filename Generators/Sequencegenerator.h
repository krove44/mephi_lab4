#pragma once
#include <memory>
#include "IGenerator.h"
#include "../Cardinal.h"
#include "../LazySequence.h"
#include "../Exception/SequenceGeneratorException.h"

template <template <typename> class Container, typename T>
class SequenceGenerator : public IGenerator<Container, T> {
private:
    LazySequence<Container, T> seq_;
    size_t cursor_ = 0;
    size_t offset_ = 0;

public:
    SequenceGenerator(const LazySequence<Container, T>& seq, size_t offset = 0): 
        seq_(seq), cursor_(offset), offset_(offset) {}

    bool HasNext() const override {
        Cardinal size = seq_.GetLenght();
        if (size.IsInfinite()) {
            return true;
        }
        return cursor_ < size.Value();
    }

    T GetNext() override {
        if (!HasNext()) {
            throw SequenceGeneratorException("no elements");
        }
        return seq_.Get(cursor_++);
    }

    T Current() const override {
        if (cursor_ == offset_) {
            throw SequenceGeneratorException("no current");
        }
        return seq_.Get(cursor_ - 1);
    }

    T Get(Cardinal index) override {
        if (!index.IsFinite()) {
            throw SequenceGeneratorException("bad index");
        }
        return seq_.Get(offset_ + index.Value());
    }

    Cardinal Size() const override {
        Cardinal total = seq_.GetLenght();
        if (total.IsInfinite()) {
            return Cardinal::Omega();
        }
        size_t total_v = total.Value();
        if (total_v <= offset_) {
            return Cardinal(size_t(0));
        }
        return Cardinal(total_v - offset_);
    }

    std::shared_ptr<IGenerator<Container, T>> Slice(Cardinal offset) const override {
        if (!offset.IsFinite()) {
            throw SequenceGeneratorException("bad offset");
        }
        return std::make_shared<SequenceGenerator<Container, T>>(seq_, offset_ + offset.Value());
    }
};