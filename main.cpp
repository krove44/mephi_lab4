#include <cstddef>
#include <iostream>
#include <memory>
#include "Generator.h"
#include "LazySequence.h"
#include "mephi_lab2/Sequence/ListSequence.h"


int main() {
    try{
    LazySequence<ListSequence, int> sp(std::make_shared<Generator<ListSequence, int>>([](const ListSequence<int>& cache){
        size_t size = cache.GetLenght();
        return cache[size-2] + cache[size - 1];}, ListSequence<int>({0, 1})));
    std::cout << sp.Get(20) << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    } catch (...) {
        std::cerr << "uknow" << std::endl;
    }

    return 0;
}