#pragma once
#include <cstddef>


class Cardinal {
private:
    bool is_infinite_;
    size_t value_;
public:
    Cardinal(bool arg) : is_infinite_(arg), value_{}{};

    Cardinal() : is_infinite_(false), value_(0) {};
    
    Cardinal(size_t value) : is_infinite_(false), value_(value) {};
    
    bool IsInfinite() const { 
        return is_infinite_;
    }

};