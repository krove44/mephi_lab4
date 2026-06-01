#include <cstddef>
#include <iostream>
#include <memory>
#include "LazySequence.h"
#include "mephi_lab2/Sequence/ListSequence.h"


int main() {
    try{
    // LazySequence<ListSequence, int> sp(std::make_shared<Generator<ListSequence, int>>([](const ListSequence<int>& cache){
    //     size_t size = cache.GetLenght();
    //     return cache[size-2] + cache[size - 1];}, ListSequence<int>({0, 1})));
    LazySequence<ListSequence, int> sp = {1 ,2 ,3 ,4};
    auto sp1 = sp.Append(110);    
    std::cout << sp1.Get(1) << std::endl;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    } catch (...) {
        std::cerr << "uknow" << std::endl;
    }

    return 0;
}