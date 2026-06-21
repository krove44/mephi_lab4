#include <gtest/gtest.h>
#include <memory>
#include "../Cardinal.h"
#include "../LazySequence.h"
#include "../Streams/LazySequenceStream.h"
#include "../OnlineStatistic.h"
#include "../Generators/IGenerator.h"
#include "../Generators/RecurentGenerator.h"
#include "../mephi_lab2/Sequence/ArraySequence.h"

template <template <typename> class Container>
LazySequence<Container, int> MakeFibSeq() {
    Container<int> seed = {0, 1};
    auto rule = [](const Container<int>& c) -> int {
        size_t n = c.GetLenght();
        return c[n - 1] + c[n - 2];
    };
    auto gen = std::make_shared<RecurentGenerator<Container, int>>(rule, seed);
    return LazySequence<Container, int>(std::static_pointer_cast<IGenerator<Container, int>>(gen));
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

