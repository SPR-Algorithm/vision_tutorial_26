//
// Created by Hacker on 25-9-6.
//
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Vector.h"
using namespace algebra;

// 要测试哪个函数，就把对应的测试取消注释

/*
TEST(VectorTest, Zeros) {
    Vector v = zeros(5);
    ASSERT_EQ(v.size(), 5);
    for (auto x : v) EXPECT_DOUBLE_EQ(x, 0);
}

TEST(VectorTest, Ones) {
    Vector v = ones(3);
    ASSERT_EQ(v.size(), 3);
    for (auto x : v) EXPECT_DOUBLE_EQ(x, 1);
}

TEST(VectorTest, Random) {
    Vector v = random(10, -2, 2);
    ASSERT_EQ(v.size(), 10);
    for (auto x : v) {
    EXPECT_GE(x, -2);
    EXPECT_LT(x, 2);
    }
    EXPECT_THROW(random(5, 3, 1), std::logic_error);
}

TEST(VectorTest, Show) {
    Vector v = {1.23456, 2.34567, 3.45678};
    // 这里只是调用 show，人工检查输出格式
    show(v);
}

TEST(VectorTest, SumScalar) {
    Vector v = {1, 2, 3};
    Vector res = sum(v, 2.5);
    ASSERT_EQ(res.size(), 3);
    EXPECT_DOUBLE_EQ(res[0], 3.5);
    EXPECT_DOUBLE_EQ(res[1], 4.5);
    EXPECT_DOUBLE_EQ(res[2], 5.5);
}

TEST(VectorTest, SumVector) {
    Vector v1 = {1, 2, 3};
    Vector v2 = {4, 5, 6};
    Vector res = sum(v1, v2);
    ASSERT_EQ(res.size(), 3);
    EXPECT_DOUBLE_EQ(res[0], 5);
    EXPECT_DOUBLE_EQ(res[1], 7);
    EXPECT_DOUBLE_EQ(res[2], 9);
    EXPECT_THROW(sum(v1, Vector{1,2}), std::logic_error);
}

TEST(VectorTest, MultiplyScalar) {
    Vector v = {1, -2, 3};
    Vector res = multiply(v, 2);
    ASSERT_EQ(res.size(), 3);
    EXPECT_DOUBLE_EQ(res[0], 2);
    EXPECT_DOUBLE_EQ(res[1], -4);
    EXPECT_DOUBLE_EQ(res[2], 6);
}

TEST(VectorTest, DotProduct) {
    Vector v1 = {1, 2, 3};
    Vector v2 = {4, 5, 6};
    double res = multiply(v1, v2);
    EXPECT_DOUBLE_EQ(res, 1*4 + 2*5 + 3*6);
    EXPECT_THROW(multiply(v1, Vector{1,2}), std::logic_error);
}

TEST(VectorTest, Norm) {
    Vector v = {3, 4};
    EXPECT_DOUBLE_EQ(norm(v), 5);
    EXPECT_DOUBLE_EQ(norm(Vector{}), 0);
}

TEST(VectorTest, Normalize) {
    Vector v = {3, 4};
    Vector res = normalize(v);
    EXPECT_NEAR(res[0], 0.6, 1e-6);
    EXPECT_NEAR(res[1], 0.8, 1e-6);
    EXPECT_THROW(normalize(Vector{}), std::logic_error);
}

TEST(VectorTest, Concatenate) {
    Vector v1 = {1, 2};
    Vector v2 = {3, 4, 5};
    Vector res = concatenate(v1, v2);
    ASSERT_EQ(res.size(), 5);
    EXPECT_DOUBLE_EQ(res[0], 1);
    EXPECT_DOUBLE_EQ(res[2], 3);
    EXPECT_DOUBLE_EQ(res[4], 5);
}*/