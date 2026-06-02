#include <gtest/gtest.h>
#include <memory>
#include "../Cardinal.h"
#include "../LazySequence.h"
#include "../Generators/IGenerator.h"
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
LazySequence<Container, int> MakeFibSeq() {
    Container<int> seed = {0, 1};
    auto gen = std::make_shared<RecurentGenerator<Container, int>>(
        FibRule<Container>(), seed);
    return LazySequence<Container, int>(
        std::static_pointer_cast<IGenerator<Container, int>>(gen));
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
