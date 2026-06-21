#pragma once
#include "IReadOnlyStream.h"
#include "../LazySequence.h"
#include "../Cardinal.h"

template <template <typename> class Container, typename T>
class LazySequenceStream : public IReadOnlyStream<T> {
private:
    LazySequence<Container, T> seq_;
    size_t position_;
    size_t limit_;
    bool has_limit_;

public:
    explicit LazySequenceStream(const LazySequence<Container, T>& seq)
        : seq_(seq), position_(0), limit_(0), has_limit_(false) {}

    LazySequenceStream(const LazySequence<Container, T>& seq, size_t limit)
        : seq_(seq), position_(0), limit_(limit), has_limit_(true) {}

    Optional<T> try_read() override {
        if (has_limit_ && position_ >= limit_) {
            return Optional<T>();
        }

        if (!seq_.IsInfinite()) {
            Cardinal len = seq_.GetLenght();
            if (len.IsFinite() && position_ >= len.Value()) {
                return Optional<T>();
            }
        }

        try {
            T value = seq_.Get(position_);
            position_++;
            return Optional<T>(value);
        } catch (...) {
            return Optional<T>();
        }
    }

    size_t GetPosition() const {
        return position_;
    }

    void Reset() {
        position_ = 0;
    }

    void SetLimit(size_t limit) {
        limit_ = limit;
        has_limit_ = true;
    }

    void RemoveLimit() {
        limit_ = 0;
        has_limit_ = false;
    }

    bool HasLimit() const {
        return has_limit_;
    }

    size_t GetLimit() const {
        return limit_;
    }
};