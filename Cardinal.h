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

    Cardinal(size_t value) : type_(Type::Finite), value_(value) {}

    Cardinal(bool is_infinite) : type_(is_infinite ? Type::Infinite : Type::Finite), value_(0) {}
    
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
        if (IsFinite()) {
            throw CardinalException("Value on no finite");
        }
        return value_;
    }

    size_t FinitePart() const {
        if (IsTransfinite()) {
            throw CardinalException("FinitePart no transfinite");
        }
        return value_;
    }

    
};