#include <cstddef>
#include <iostream>
#include <memory>
#include "LazySequence.h"
#include "mephi_lab2/Sequence/ListSequence.h"


int main() {
    LazySequence<ListSequence, int> sp(std::make_shared<[](const ListSequence<int>& cache){
        size_t size = cache.GetLenght();
        return cache[size-2] + cache[size - 1];}>, {0, 1});
    std::cout << sp.Get(2) << std::endl;
    return 0;
}