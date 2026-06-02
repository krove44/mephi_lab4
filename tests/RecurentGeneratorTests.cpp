#include <gtest/gtest.h>
#include <functional>
#include <memory>
#include "../Cardinal.h"
#include "../Generators/RecurentGenerator.h"
#include "../mephi_lab2/Sequence/ArraySequence.h"
#include "../mephi_lab2/Sequence/ListSequence.h"

template <template <typename> class Container>
auto FibRule() {
    return [](const Container<int>& c) -> int {
        size_t n = c.GetLenght();
        return c[n - 1] + c[n - 2];
    };
}

template <template <typename> class Container>
auto ArithmeticRule(int d) {
    return [d](const Container<int>& c) -> int {
        size_t n = c.GetLenght();
        return c[n - 1] + d;
    };
}


TEST(RecurentGeneratorArray, InfiniteSizeIsOmega) {
    ArraySequence<int> seed = {0, 1};
    RecurentGenerator<ArraySequence, int> gen(FibRule<ArraySequence>(), seed);
    EXPECT_TRUE(gen.Size().IsInfinite());
}

TEST(RecurentGeneratorArray, FiniteSizeCorrect) {
    ArraySequence<int> seed = {0, 1};
    RecurentGenerator<ArraySequence, int> gen(FibRule<ArraySequence>(), seed, 10);
    EXPECT_EQ(gen.Size().Value(), size_t(10));
}

TEST(RecurentGeneratorArray, HasNextInfiniteAlwaysTrue) {
    ArraySequence<int> seed = {0, 1};
    RecurentGenerator<ArraySequence, int> gen(FibRule<ArraySequence>(), seed);
    EXPECT_TRUE(gen.HasNext());
}

TEST(RecurentGeneratorArray, HasNextFiniteExhausts) {
    ArraySequence<int> seed = {0, 1};
    RecurentGenerator<ArraySequence, int> gen(FibRule<ArraySequence>(), seed, 2);
    gen.GetNext();
    gen.GetNext();
    EXPECT_FALSE(gen.HasNext());
}

TEST(RecurentGeneratorArray, GetNextReturnsFibonacci) {
    ArraySequence<int> seed = {0, 1};
    RecurentGenerator<ArraySequence, int> gen(FibRule<ArraySequence>(), seed);
    EXPECT_EQ(gen.GetNext(), 0);
    EXPECT_EQ(gen.GetNext(), 1);
    EXPECT_EQ(gen.GetNext(), 1);
    EXPECT_EQ(gen.GetNext(), 2);
    EXPECT_EQ(gen.GetNext(), 3);
    EXPECT_EQ(gen.GetNext(), 5);
}

TEST(RecurentGeneratorArray, GetByIndexNonSequential) {
    ArraySequence<int> seed = {0, 1};
    RecurentGenerator<ArraySequence, int> gen(FibRule<ArraySequence>(), seed);
    EXPECT_EQ(gen.Get(Cardinal(6u)), 8);
    EXPECT_EQ(gen.Get(Cardinal(0u)), 0);
    EXPECT_EQ(gen.Get(Cardinal(9u)), 34);
}
