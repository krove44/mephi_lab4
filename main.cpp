#include <iostream>
#include "LazySequence.h"
#include "mephi_lab2/Sequence/ListSequence.h"


int main() {
    int arr[3] = {1, 2, 3};
    LazySequence<int> sp(ListSequence<int>(std::span(arr, 3)),[](int x1, int x2){return x1 + x2;}, 2);

    return 0;
}