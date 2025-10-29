#include <gtest/gtest.h>
#include <stdexcept>
#include "../vector.h"

class VectorCheck : public ::testing::Test {
protected:
    void SetUp() override {
        vec.Push_back(1);
        vec.Push_back(2);
        vec.Push_back(3);
    }

    void TearDown() override {}

    Vector<int> vec;
    Vector<int> empty_vec;
};

TEST_F(VectorCheck, Intial) {
    EXPECT_EQ(vec.Size(), 3);
    EXPECT_FALSE(vec.Empty());
}

TEST_F(VectorCheck, AccessingElements) {
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec.At(1), 2);
    EXPECT_EQ(vec.Front(), 1);
    EXPECT_EQ(vec.Back(), 3);
}

TEST_F(VectorCheck, OutOf) {
    EXPECT_THROW(empty_vec.At(1), std::out_of_range);
}

TEST(CopyTest, DeepCopy) {
    Vector<int> orig = {1, 2, 3};
    Vector<int> copy = orig;

    copy[0] = 52;

    EXPECT_EQ(orig[0], 1);
    EXPECT_EQ(copy[0], 52);
    EXPECT_EQ(copy.Size(), orig.Size());
}

TEST(MoveTest, Move) {
    Vector<int> orig = {1, 2, 3};
    Vector<int> moved = std::move(orig);

    EXPECT_EQ(moved.Size(), 3);
    EXPECT_EQ(orig.Size(), 0);
    EXPECT_EQ(moved[1], 2);
    EXPECT_TRUE(orig.Empty());
}

TEST(SelfCopy, Copy) {
    Vector<int> orig = {1, 2, 3};
    orig = orig;
    EXPECT_FALSE(orig.Empty());
    EXPECT_EQ(orig.Size(), 3);
    EXPECT_EQ(orig[0], 1);
}

TEST(MemoryTest1, ReserveTest) {
    Vector<int> orig = {1, 2, 3};
    orig.Reserve(5);
    EXPECT_EQ(orig.Size(), 3);
    EXPECT_EQ(orig.Capacity(), 5);
    EXPECT_TRUE((orig[0] == 1) && (orig[1] == 2) && (orig[2] == 3));
}

TEST(MemoryTest2, ResizeTest1) {
    Vector<int> orig = {1, 2, 3};
    orig.Resize(4, 3);
    EXPECT_EQ(orig.Size(), 4);
    EXPECT_EQ(orig.Capacity(), 4);
    EXPECT_TRUE((orig[0] == 1) && (orig[1] == 2) && (orig[2] == 3));
    EXPECT_EQ(orig[3], 3);
}

TEST(MemoryTest3, ResizeTest2) {
    Vector<int> orig = {1, 2, 3, 4};
    orig.Resize(2);
    EXPECT_EQ(orig.Size(), 2);
    EXPECT_EQ(orig.Capacity(), 4);
    EXPECT_TRUE((orig[0] == 1) && (orig[1] == 2));
}

TEST(ModificationTest1, PushAndPopTest) {
    Vector<int> orig = {1, 2, 3};
    orig.Push_back(4);
    EXPECT_EQ(orig.Size(), 4);
    EXPECT_EQ(orig.Capacity(), 6);
    EXPECT_EQ(orig[3],4);
    orig.Pop_back();
    EXPECT_EQ(orig.Size(), 3);
    EXPECT_EQ(orig.Capacity(), 6);
    EXPECT_THROW(orig.At(3), std::out_of_range);
}

TEST(ModificationTest2, InsertAndEraseTest) {
    Vector<int> orig = {1, 2, 3};
    orig.Insert(1, 4);
    EXPECT_EQ(orig.Size(), 4);
    EXPECT_EQ(orig.Capacity(), 6);
    EXPECT_EQ(orig[1],4);
    EXPECT_TRUE((orig[0] == 1) && (orig[2] == 2) && (orig[3] == 3));
    orig.Erase(1);
    EXPECT_EQ(orig.Size(), 3);
    EXPECT_EQ(orig.Capacity(), 6);
    EXPECT_TRUE((orig[0] == 1) && (orig[1] == 2) && (orig[2] == 3));
}

TEST(ModificationTest3, ClearTest) {
    Vector<int> orig = {1, 2, 3};
    orig.Clear();
    EXPECT_EQ(orig.Size(), 0);
    EXPECT_EQ(orig.Capacity(), 3);
    EXPECT_THROW(orig.At(0), std::out_of_range);
}