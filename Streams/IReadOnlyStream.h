#pragma once
#include "..\Optional.h"

template<typename T>
class IReadOnlyStream {
public:
    virtual ~IReadOnlyStream() = default;
    virtual Optional<T> try_read() = 0;
};