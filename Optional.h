#pragma once
#include <cstddef>
#include "Exception/OptionalException.h"

template<typename T>
class Optional {
private:
    alignas(T) std::byte storage_[sizeof(T)];
    bool hasValue_;

    T* ptr() {
        return reinterpret_cast<T*>(storage_);
    }

public:
    Optional() : storage_(), hasValue_(false) {}

    Optional(const T& value) : hasValue_(true) {
        new (storage_) T(value);
    }

    Optional(Optional&& other) : hasValue_(other.hasValue_) {
        if (hasValue_) {
            new (storage_) T(std::move(*other.ptr()));
            other.ptr()->~T();
            other.hasValue_ = false;
        }
    }

    ~Optional() {
        if (hasValue_) {
            ptr()->~T();
        }
    }

    bool has_value() const {
        return hasValue_;
    }

    T value() {
        if (!hasValue_) {
            throw OptionalEmpthy("Optional is Empthy");
        }
        return *ptr();
    }

    T* operator->() {
        return ptr();
    }
};