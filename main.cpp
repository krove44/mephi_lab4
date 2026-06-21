#include <iostream>
#include <fstream>
#include "Streams/FileReadOnlyStream.h"
#include "OnlineStatistic.h"

int main() {
    ReadOnlyStream<int> stream("t.txt");
    Statistics<int> stats;
    stats.read_from_stream(stream, 0);

    std::cout << stats.count() << "\n"
              << stats.sum() << "\n"
              << stats.min() << "\n"
              << stats.max() << "\n"
              << stats.get_median() << "\n";

    return 0;
}