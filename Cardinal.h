#pragma once
#include "Exception/CardinalException.h"
#include <cstddef>

enum class Type {
        Finite,
        Infinite,
        Transfinite
    };

class Cardinal {
private:
    Type type_;
    size_t value_;
public:
    Cardinal() : type_(Type::Finite), value_(0) {}

    Cardinal(Type type, size_t value) : type_(type), value_(value){};

    Cardinal(Type type) = delete;

    Cardinal(size_t value) : type_(Type::Finite), value_(value) {}

    
    static Cardinal Omega() {
        return Cardinal(Type::Infinite, 0);
    }

    static Cardinal OmegaPlus(size_t k) {
        return Cardinal(Type::Transfinite, k);
    }

    bool IsFinite() const {return type_ == Type::Finite;}
    bool IsInfinite() const {return type_ == Type::Infinite;}
    bool IsTransfinite() const {return type_ == Type::Transfinite;}

    size_t Value() const {
        if (IsInfinite()) {
            throw CardinalException("Value on no finite");
        }
        return value_;
    }

    size_t FinitePart() const {
        if (IsInfinite()) {
            throw CardinalException("FinitePart no IsInfinite");
        }
        return value_;
    }

    bool operator<(const Cardinal& other) const {
        if (type_ == Type::Finite) {
            if (other.type_ == Type::Finite) {
                return value_ < other.value_;
            }
            return true;
        }
        if (type_ == Type::Infinite) {
            if (other.type_ == Type::Finite) return false;
            if (other.type_ == Type::Infinite) return false;
            return true;
        }
        if (other.type_ == Type::Transfinite) {
            return value_ < other.value_;
        }
        return false;
    }

    bool operator==(const Cardinal& other) const {
        return type_ == other.type_ && value_ == other.value_;
    }

    bool operator!=(const Cardinal& other) const { return !(*this == other); }
    bool operator<=(const Cardinal& other) const { return *this < other || *this == other; }
    bool operator>(const Cardinal& other) const { return other < *this; }
    bool operator>=(const Cardinal& other) const { return other <= *this; }

    Cardinal operator-(const Cardinal& other) const {
        if (type_ == Type::Finite && other.type_ == Type::Finite) {
            return Cardinal(value_ - other.value_);
        }
        if (type_ == Type::Transfinite && other.type_ == Type::Infinite) {
            return Cardinal(value_);
        }
        throw CardinalException("No good Type");
    }

    Cardinal operator+(const Cardinal& other) const {
        if (type_ == Type::Finite && other.type_ == Type::Finite) {
            return Cardinal(value_ + other.value_);
        }
        return Cardinal::Omega();
    }
};