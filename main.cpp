#include <iostream>
#include "LazySequence.h"
#include "mephi_lab2/Sequence/ListSequence.h"


int main() {
    LazySequence<ListSequence, int> sp = {4, 2, 3};
    std::cout << sp.GetFirst() << std::endl;
    return 0;
}