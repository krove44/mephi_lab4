#include <gtest/gtest.h>
#include "../Cardinal.h"

TEST(Cardinal, DefaultConstructorIsFiniteZero) {
    Cardinal c;
    EXPECT_TRUE(c.IsFinite());
    EXPECT_EQ(c.Value(), size_t(0));
}

TEST(Cardinal, SizeConstructorStoresValue) {
    Cardinal c(42);
    EXPECT_TRUE(c.IsFinite());
    EXPECT_EQ(c.Value(), size_t(42));
}

TEST(Cardinal, OmegaIsInfinite) {
    Cardinal w = Cardinal::Omega();
    EXPECT_TRUE(w.IsInfinite());
    EXPECT_FALSE(w.IsFinite());
    EXPECT_FALSE(w.IsTransfinite());
}

TEST(Cardinal, OmegaPlusIsTransfinite) {
    Cardinal wp = Cardinal::OmegaPlus(7);
    EXPECT_TRUE(wp.IsTransfinite());
    EXPECT_FALSE(wp.IsFinite());
    EXPECT_FALSE(wp.IsInfinite());
    EXPECT_EQ(wp.Value(), size_t(7));
}

TEST(Cardinal, FiniteAddition) {
    Cardinal a(3), b(4);
    Cardinal s = a + b;
    EXPECT_TRUE(s.IsFinite());
    EXPECT_EQ(s.Value(), size_t(7));
}

TEST(Cardinal, FinitePlusInfiniteIsInfinite) {
    Cardinal a(10), w = Cardinal::Omega();
    Cardinal s = a + w;
    EXPECT_TRUE(s.IsInfinite());
}

TEST(Cardinal, InfinitePlusFiniteIsInfinite) {
    Cardinal w = Cardinal::Omega();
    Cardinal s = w + Cardinal(5);
    EXPECT_TRUE(s.IsInfinite());
}

TEST(Cardinal, FiniteSubtraction) {
    Cardinal a(10), b(3);
    Cardinal d = a - b;
    EXPECT_TRUE(d.IsFinite());
    EXPECT_EQ(d.Value(), size_t(7));
}

TEST(Cardinal, TransfiniteMinusInfinite) {
    Cardinal wp = Cardinal::OmegaPlus(5);
    Cardinal w  = Cardinal::Omega();
    Cardinal d  = wp - w;
    EXPECT_TRUE(d.IsFinite());
    EXPECT_EQ(d.Value(), size_t(5));
}

TEST(Cardinal, ComparisonFiniteLess) {
    EXPECT_TRUE(Cardinal(1)  < Cardinal(2));
    EXPECT_FALSE(Cardinal(2) < Cardinal(1));
    EXPECT_FALSE(Cardinal(2) < Cardinal(2));
}

TEST(Cardinal, FiniteLessThanInfinite) {
    EXPECT_TRUE(Cardinal(9999u) < Cardinal::Omega());
}

TEST(Cardinal, InfiniteNotLessThanFinite) {
    EXPECT_FALSE(Cardinal::Omega() < Cardinal(9999));
}

TEST(Cardinal, InfiniteNotLessThanInfinite) {
    EXPECT_FALSE(Cardinal::Omega() < Cardinal::Omega());
}

