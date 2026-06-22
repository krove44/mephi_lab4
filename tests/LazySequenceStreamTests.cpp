#include <gtest/gtest.h>
#include <memory>
#include "../LazySequence.h"
#include "../Streams/LazySequenceStream.h"
#include "../OnlineStatistic.h"
#include "../Generators/RecurentGenerator.h"
#include "../mephi_lab2/Sequence/ArraySequence.h"
#include "../mephi_lab2/Sequence/ListSequence.h"

template <template <typename> class Container>
LazySequence<Container, int> MakeFibSeq() {
    Container<int> seed = {0, 1};
    auto rule = [](const Container<int>& c) -> int {
        size_t n = c.GetLenght();
        return c[n - 1] + c[n - 2];
    };
    auto gen = std::make_shared<RecurentGenerator<Container, int>>(rule, seed);
    return LazySequence<Container, int>(gen);
}

TEST(LazySequenceStream, FiniteReadAllElements) {
    LazySequence<ArraySequence, int> seq({1, 2, 3});
    LazySequenceStream<ArraySequence, int> stream(seq);

    for (int expected : {1, 2, 3}) {
        auto opt = stream.try_read();
        ASSERT_TRUE(opt.has_value());
        EXPECT_EQ(opt.value(), expected);
    }
    EXPECT_FALSE(stream.try_read().has_value());
}

TEST(LazySequenceStream, FiniteEmptySequence) {
    LazySequence<ArraySequence, int> seq;
    LazySequenceStream<ArraySequence, int> stream(seq);
    EXPECT_FALSE(stream.try_read().has_value());
}
 
TEST(LazySequenceStream, ResetResetsPosition) {
    LazySequence<ArraySequence, int> seq({1, 2, 3});
    LazySequenceStream<ArraySequence, int> stream(seq);
 
    stream.try_read();
    stream.try_read();
    stream.Reset();
 
    EXPECT_EQ(stream.GetPosition(), size_t(0));
    EXPECT_EQ(stream.try_read().value(), 1);
}
 
TEST(LazySequenceStream, InfiniteFibWithLimit) {
    auto fib = MakeFibSeq<ArraySequence>();
    LazySequenceStream<ArraySequence, int> stream(fib, 6);
 
    for (int expected : {0, 1, 1, 2, 3, 5}) {
        auto opt = stream.try_read();
        ASSERT_TRUE(opt.has_value());
        EXPECT_EQ(opt.value(), expected);
    }
    EXPECT_FALSE(stream.try_read().has_value());
}
 
TEST(LazySequenceStream, SetLimitAfterConstruction) {
    auto fib = MakeFibSeq<ArraySequence>();
    LazySequenceStream<ArraySequence, int> stream(fib);
    stream.SetLimit(3);
 
    int count = 0;
    while (stream.try_read().has_value()) count++;
    EXPECT_EQ(count, 3);
}
 
TEST(LazySequenceStream, LimitLargerThanSequence) {
    LazySequence<ArraySequence, int> seq({1, 2, 3});
    LazySequenceStream<ArraySequence, int> stream(seq, 100);
 
    int count = 0;
    while (stream.try_read().has_value()) count++;
    EXPECT_EQ(count, 3);
}
 
TEST(LazySequenceStream, StatisticsFromFiniteSequence) {
    LazySequence<ArraySequence, int> seq({3, 1, 4, 1, 5, 9, 2, 6});
    LazySequenceStream<ArraySequence, int> stream(seq);
 
    Statistics<int> stats;
    stats.read_from_stream(stream);
 
    EXPECT_EQ(stats.count(), size_t(8));
    EXPECT_EQ(stats.sum(), 31);
    EXPECT_EQ(stats.min(), 1);
    EXPECT_EQ(stats.max(), 9);
}
 
TEST(LazySequenceStream, StatisticsFromInfiniteWithLimit) {
    auto fib = MakeFibSeq<ArraySequence>();
    LazySequenceStream<ArraySequence, int> stream(fib, 10);
 
    Statistics<int> stats;
    stats.read_from_stream(stream);
 
    EXPECT_EQ(stats.count(), size_t(10));
    EXPECT_EQ(stats.sum(), 88);
    EXPECT_EQ(stats.min(), 0);
    EXPECT_EQ(stats.max(), 34);
}
 
TEST(LazySequenceStream, MedianFromStream) {
    LazySequence<ArraySequence, int> seq({5, 1, 3, 2, 4});
    LazySequenceStream<ArraySequence, int> stream(seq);
 
    Statistics<int> stats;
    stats.read_from_stream(stream);
 
    EXPECT_DOUBLE_EQ(stats.get_median(), 3.0);
}
 
TEST(LazySequenceStreamList, InfiniteFibWithLimit) {
    auto fib = MakeFibSeq<ListSequence>();
    LazySequenceStream<ListSequence, int> stream(fib, 5);
 
    for (int expected : {0, 1, 1, 2, 3}) {
        auto opt = stream.try_read();
        ASSERT_TRUE(opt.has_value());
        EXPECT_EQ(opt.value(), expected);
    }
    EXPECT_FALSE(stream.try_read().has_value());
}

