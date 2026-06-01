#pragma once
#include <memory>
#include "..\Cardinal.h"

template <template <typename> class Container, typename T>
class IGenerator {
public:
    virtual ~IGenerator() = default;
    virtual bool HasNext() const = 0;
    virtual T GetNext() = 0;
    virtual T Current() const = 0;    
    virtual T Get(Cardinal index) = 0;
    virtual Cardinal Size() const = 0;
    virtual std::shared_ptr<IGenerator<Container,T>> Slice(Cardinal offset) const = 0;
};