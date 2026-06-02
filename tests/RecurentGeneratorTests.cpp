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


TEST(RecurentGeneratorArray, CurrentAfterGetNext) {
    ArraySequence<int> seed = {0, 1};
    RecurentGenerator<ArraySequence, int> gen(FibRule<ArraySequence>(), seed);
    gen.GetNext();
    gen.GetNext();
    EXPECT_EQ(gen.Current(), 1);
}

TEST(RecurentGeneratorArray, CurrentBeforeGetNextThrows) {
    ArraySequence<int> seed = {0, 1};
    RecurentGenerator<ArraySequence, int> gen(FibRule<ArraySequence>(), seed);
    EXPECT_THROW(gen.Current(), RecurentGeneratorException);
}

TEST(RecurentGeneratorArray, GetWithInfiniteIndexThrows) {
    ArraySequence<int> seed = {0, 1};
    RecurentGenerator<ArraySequence, int> gen(FibRule<ArraySequence>(), seed);
    EXPECT_THROW(gen.Get(Cardinal::Omega()), RecurentGeneratorException);
}

TEST(RecurentGeneratorArray, FiniteGetNextBeyondSizeThrows) {
    ArraySequence<int> seed = {0, 1};
    RecurentGenerator<ArraySequence, int> gen(FibRule<ArraySequence>(), seed, 2);
    gen.GetNext();
    gen.GetNext();
    EXPECT_THROW(gen.GetNext(), RecurentGeneratorException);
}

TEST(RecurentGeneratorArray, SlicePreservesValues) {
    ArraySequence<int> seed = {0, 1};
    RecurentGenerator<ArraySequence, int> gen(FibRule<ArraySequence>(), seed);
    auto sliced = gen.Slice(Cardinal(2));
    EXPECT_EQ(sliced->Get(Cardinal(0u)), 1);
    EXPECT_EQ(sliced->Get(Cardinal(1u)), 2);
}

TEST(RecurentGeneratorArray, SliceWithInfiniteOffsetThrows) {
    ArraySequence<int> seed = {0, 1};
    RecurentGenerator<ArraySequence, int> gen(FibRule<ArraySequence>(), seed);
    EXPECT_THROW(gen.Slice(Cardinal::Omega()), RecurentGeneratorException);
}

TEST(RecurentGeneratorArray, ArithmeticProgressionGetNext) {
    ArraySequence<int> seed = {0};
    RecurentGenerator<ArraySequence, int> gen(ArithmeticRule<ArraySequence>(3), seed);
    EXPECT_EQ(gen.GetNext(), 0);
    EXPECT_EQ(gen.GetNext(), 3);
    EXPECT_EQ(gen.GetNext(), 6);
    EXPECT_EQ(gen.GetNext(), 9);
}

TEST(RecurentGeneratorArray, ArithmeticProgressionGetByIndex) {
    ArraySequence<int> seed = {0};
    RecurentGenerator<ArraySequence, int> gen(ArithmeticRule<ArraySequence>(5), seed);
    EXPECT_EQ(gen.Get(Cardinal(4u)), 20);
}

TEST(RecurentGeneratorArray, FiniteSliceHasCorrectSize) {
    ArraySequence<int> seed = {0, 1};
    RecurentGenerator<ArraySequence, int> gen(FibRule<ArraySequence>(), seed, 8);
    auto sliced = gen.Slice(Cardinal(3u));
    EXPECT_EQ(sliced->Size().Value(), size_t(5));
}

