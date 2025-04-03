#include <gtest/gtest.h>
#include "../include/deque.hpp"

namespace my_container {
    namespace {

        class DequeTest : public ::testing::Test {
        protected:
            Deque<int> empty_deque;
            Deque<int> sample_deque;

            DequeTest() : sample_deque{1, 2, 3} {}
        };

        TEST_F(DequeTest, DefaultConstructor) {
            EXPECT_TRUE(empty_deque.empty());
            EXPECT_EQ(empty_deque.size(), 0);
        }

        TEST_F(DequeTest, InitializerListConstructor) {
            EXPECT_EQ(sample_deque.size(), 3);
            EXPECT_EQ(sample_deque.front(), 1);
            EXPECT_EQ(sample_deque.back(), 3);
        }

        TEST_F(DequeTest, PushBack) {
            sample_deque.push_back(4);
            EXPECT_EQ(sample_deque.back(), 4);
            EXPECT_EQ(sample_deque.size(), 4);
        }

        TEST_F(DequeTest, PushFront) {
            sample_deque.push_front(0);
            EXPECT_EQ(sample_deque.front(), 0);
            EXPECT_EQ(sample_deque.size(), 4);
        }

        TEST_F(DequeTest, PopBack) {
            sample_deque.pop_back();
            EXPECT_EQ(sample_deque.back(), 2);
            EXPECT_EQ(sample_deque.size(), 2);
        }

        TEST_F(DequeTest, PopFront) {
            sample_deque.pop_front();
            EXPECT_EQ(sample_deque.front(), 2);
            EXPECT_EQ(sample_deque.size(), 2);
        }

        TEST_F(DequeTest, FrontBackThrowWhenEmpty) {
            EXPECT_THROW(empty_deque.front(), std::out_of_range);
            EXPECT_THROW(empty_deque.back(), std::out_of_range);
        }

        TEST_F(DequeTest, Iterators) {
            auto it = sample_deque.begin();
            EXPECT_EQ(*it, 1);
            ++it;
            EXPECT_EQ(*it, 2);
            ++it;
            EXPECT_EQ(*it, 3);
            ++it;
            EXPECT_EQ(it, sample_deque.end());
        }

        TEST_F(DequeTest, CopyConstructor) {
            Deque<int> copy(sample_deque);
            EXPECT_EQ(copy.size(), 3);
            EXPECT_EQ(copy, sample_deque);
        }

        TEST_F(DequeTest, MoveConstructor) {
            Deque<int> moved(std::move(sample_deque));
            EXPECT_EQ(moved.size(), 3);
            EXPECT_TRUE(sample_deque.empty());
        }

        TEST_F(DequeTest, MaxSize) {
            EXPECT_EQ(empty_deque.max_size(), std::numeric_limits<size_t>::max());
        }


    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
