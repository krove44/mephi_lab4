#include <gtest/gtest.h>
#include <memory>
#include "../Cardinal.h"
#include "../LazySequence.h"
#include "../Generators/RecurentGenerator.h"
#include "../mephi_lab2/Sequence/ArraySequence.h"

template <template <typename> class Container>
auto FibRule() {
    return [](const Container<int>& c) -> int {
        size_t n = c.GetLenght();
        return c[n - 1] + c[n - 2];
    };
}

template <template <typename> class Container>
LazySequence<Container, int> MakeFibSeq() {
    Container<int> seed = {0, 1};
    auto gen = std::make_shared<RecurentGenerator<Container, int>>(
        FibRule<Container>(), seed);
    return LazySequence<Container, int>(gen);
}

TEST(LazySequenceArray, DefaultConstructorIsEmpty) {
    LazySequence<ArraySequence, int> seq;
    EXPECT_EQ(seq.GetLenght().Value(), size_t(0));
}

TEST(LazySequenceArray, ContainerConstructorStoresElements) {
    LazySequence<ArraySequence, int> seq(ArraySequence<int>{1, 2, 3});
    EXPECT_EQ(seq.GetLenght().Value(), size_t(3));
    EXPECT_EQ(seq.Get(0), 1);
    EXPECT_EQ(seq.Get(2), 3);
}

TEST(LazySequenceArray, InitializerListConstructor) {
    LazySequence<ArraySequence, int> seq(ArraySequence<int>{10, 20, 30});
    EXPECT_EQ(seq.GetLenght().Value(), size_t(3));
    EXPECT_EQ(seq.Get(1), 20);
}

TEST(LazySequenceArray, GetFirst) {
    LazySequence<ArraySequence, int> seq(ArraySequence<int>{5, 6, 7});
    EXPECT_EQ(seq.GetFirst(), 5);
}

TEST(LazySequenceArray, GetLast) {
    LazySequence<ArraySequence, int> seq(ArraySequence<int>{5, 6, 7});
    EXPECT_EQ(seq.GetLast(), 7);
}

TEST(LazySequenceArray, GetLastOnEmptyThrows) {
    LazySequence<ArraySequence, int> seq;
    EXPECT_THROW(seq.GetLast(), LazySequenceIsEmpthy);
}

TEST(LazySequenceArray, AppendIncreasesSize) {
    LazySequence<ArraySequence, int> seq(ArraySequence<int>{1, 2});
    auto seq2 = seq.Append(3);
    EXPECT_EQ(seq2.GetLenght().Value(), size_t(3));
    EXPECT_EQ(seq2.GetLast(), 3);
}

TEST(LazySequenceArray, AppendIsNonMutating) {
    LazySequence<ArraySequence, int> seq(ArraySequence<int>{1, 2});
    seq.Append(3);
    EXPECT_EQ(seq.GetLenght().Value(), size_t(2));
}

TEST(LazySequenceArray, PrependAddsToFront) {
    LazySequence<ArraySequence, int> seq(ArraySequence<int>{2, 3});
    auto seq2 = seq.Prepend(1);
    EXPECT_EQ(seq2.GetFirst(), 1);
    EXPECT_EQ(seq2.GetLenght().Value(), size_t(3));
}

TEST(LazySequenceArray, PrependIsNonMutating) {
    LazySequence<ArraySequence, int> seq(ArraySequence<int>{2, 3});
    seq.Prepend(1);
    EXPECT_EQ(seq.GetFirst(), 2);
}

TEST(LazySequenceArray, InsertAtMiddle) {
    LazySequence<ArraySequence, int> seq(ArraySequence<int>{1, 3});
    auto seq2 = seq.InsertAt(2, 1);
    EXPECT_EQ(seq2.Get(0), 1);
    EXPECT_EQ(seq2.Get(1), 2);
    EXPECT_EQ(seq2.Get(2), 3);
    EXPECT_EQ(seq2.GetLenght().Value(), size_t(3));
}

TEST(LazySequenceArray, InsertAtOutOfRangeThrows) {
    LazySequence<ArraySequence, int> seq(ArraySequence<int>{1, 2});
    EXPECT_THROW(seq.InsertAt(99, 10), LazySequenceOutOfRange);
}

TEST(LazySequenceArray, GetSubsequence) {
    LazySequence<ArraySequence, int> seq(ArraySequence<int>{10, 20, 30, 40, 50});
    auto sub = seq.GetSubsequence(1, 3);
    EXPECT_EQ(sub.GetLenght().Value(), size_t(3));
    EXPECT_EQ(sub.Get(0), 20);
    EXPECT_EQ(sub.Get(2), 40);
}

TEST(LazySequenceArray, GetSubsequenceInvalidRangeThrows) {
    LazySequence<ArraySequence, int> seq(ArraySequence<int>{1, 2, 3});
    EXPECT_THROW(seq.GetSubsequence(3, 1), LazySequenceGetSubsequence);
}

TEST(LazySequenceArray, GetSubsequenceSingleElement) {
    LazySequence<ArraySequence, int> seq(ArraySequence<int>{10, 20, 30});
    auto sub = seq.GetSubsequence(1, 1);
    EXPECT_EQ(sub.GetLenght().Value(), size_t(1));
    EXPECT_EQ(sub.Get(0), 20);
}

TEST(LazySequenceArray, CopyConstructorIsIndependent) {
    LazySequence<ArraySequence, int> seq(ArraySequence<int>{1, 2, 3});
    LazySequence<ArraySequence, int> copy(seq);
    auto new_copy = copy.Append(4);
    EXPECT_EQ(seq.GetLenght().Value(), size_t(3));
    EXPECT_EQ(new_copy.GetLenght().Value(), size_t(4));
}

TEST(LazySequenceArray, ConcatTwoFiniteSequences) {
    LazySequence<ArraySequence, int> a(ArraySequence<int>{1, 2, 3});
    LazySequence<ArraySequence, int> b(ArraySequence<int>{4, 5});
    auto c = a.Concat(b);
    EXPECT_EQ(c.GetLenght().Value(), size_t(5));
    EXPECT_EQ(c.Get(0), 1);
    EXPECT_EQ(c.Get(3), 4);
    EXPECT_EQ(c.Get(4), 5);
}

TEST(LazySequenceArray, ConcatWithEmptyRight) {
    LazySequence<ArraySequence, int> a(ArraySequence<int>{1, 2});
    LazySequence<ArraySequence, int> empty;
    auto c = a.Concat(empty);
    EXPECT_EQ(c.GetLenght().Value(), size_t(2));
    EXPECT_EQ(c.Get(1), 2);
}

TEST(LazySequenceArray, ConcatWithEmptyLeft) {
    LazySequence<ArraySequence, int> empty;
    LazySequence<ArraySequence, int> b(ArraySequence<int>{7, 8});
    auto c = empty.Concat(b);
    EXPECT_EQ(c.GetLenght().Value(), size_t(2));
    EXPECT_EQ(c.Get(0), 7);
}

TEST(LazySequenceArray, ConcatPreservesOrder) {
    LazySequence<ArraySequence, int> a(ArraySequence<int>{1, 2});
    LazySequence<ArraySequence, int> b(ArraySequence<int>{3, 4});
    auto c = a.Concat(b);
    EXPECT_EQ(c.Get(0), 1);
    EXPECT_EQ(c.Get(1), 2);
    EXPECT_EQ(c.Get(2), 3);
    EXPECT_EQ(c.Get(3), 4);
}

TEST(LazySequenceArray, ConcatInfiniteWithFinite) {
    auto fib = MakeFibSeq<ArraySequence>();
    LazySequence<ArraySequence, int> finite(ArraySequence<int>{100, 200, 300});
    auto c = fib.Concat(finite);

    EXPECT_TRUE(c.IsInfinite());
    EXPECT_EQ(c.Get(0), 0);
    EXPECT_EQ(c.Get(1), 1);
    EXPECT_EQ(c.Get(6), 8);
    EXPECT_EQ(c.Get(9), 34);

    EXPECT_EQ(c.GetByCardinal(Cardinal::OmegaPlus(0)), 100);
    EXPECT_EQ(c.GetByCardinal(Cardinal::OmegaPlus(1)), 200);
    EXPECT_EQ(c.GetByCardinal(Cardinal::OmegaPlus(2)), 300);
}

TEST(LazySequenceArray, ConcatTwoInfiniteSequences) {
    auto fib = MakeFibSeq<ArraySequence>();

    ArraySequence<int> seed = {0};
    auto rule = [](const ArraySequence<int>& c) -> int {
        return c[c.GetLenght() - 1] + 10;
    };
    auto gen = std::make_shared<RecurentGenerator<ArraySequence, int>>(rule, seed);
    LazySequence<ArraySequence, int> tens(gen);

    auto c = fib.Concat(tens);

    EXPECT_TRUE(c.IsInfinite());
    EXPECT_EQ(c.Get(0), 0);
    EXPECT_EQ(c.Get(6), 8);

    EXPECT_EQ(c.GetByCardinal(Cardinal::OmegaPlus(0)), 0);
    EXPECT_EQ(c.GetByCardinal(Cardinal::OmegaPlus(1)), 10);
    EXPECT_EQ(c.GetByCardinal(Cardinal::OmegaPlus(3)), 30);
}

TEST(LazySequenceArray, InfiniteGeneratorMakesInfiniteSequence) {
    auto seq = MakeFibSeq<ArraySequence>();
    EXPECT_TRUE(seq.IsInfinite());
    EXPECT_TRUE(seq.GetLenght().IsInfinite());
}

TEST(LazySequenceArray, InfiniteSequenceGetLastThrows) {
    auto seq = MakeFibSeq<ArraySequence>();
    EXPECT_THROW(seq.GetLast(), LazySequenceIsInfinite);
}

TEST(LazySequenceArray, InfiniteSequenceRandomAccess) {
    auto seq = MakeFibSeq<ArraySequence>();
    EXPECT_EQ(seq.Get(0), 0);
    EXPECT_EQ(seq.Get(1), 1);
    EXPECT_EQ(seq.Get(6), 8);
    EXPECT_EQ(seq.Get(9), 34);
}

TEST(LazySequenceArray, GetByCardinalFinite) {
    LazySequence<ArraySequence, int> seq(ArraySequence<int>{100, 200, 300});
    EXPECT_EQ(seq.GetByCardinal(Cardinal(0u)), 100);
    EXPECT_EQ(seq.GetByCardinal(Cardinal(2u)), 300);
}
