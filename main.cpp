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
    auto sp1 = sp.GetSubsequence(0, 15);    
    for(size_t i = 0; i < 10; ++i){
        std::cout << sp1.Get(i) << std::endl;
    }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    } catch (...) {
        std::cerr << "uknow" << std::endl;
    }

    return 0;
}