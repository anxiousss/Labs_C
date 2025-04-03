#include <gtest/gtest.h>
#include "../include/Deque.hpp"

namespace my_container {
    namespace {

        class DequeTest : public ::testing::Test {
        protected:
            Deque<int> empty_deque;
            Deque<int> sample_deque;

            void SetUp() override {
                sample_deque = {10, 20, 30};
            }
        };

        // Тесты для front()
        TEST_F(DequeTest, FrontReturnsFirstElement) {
            EXPECT_EQ(sample_deque.front(), 10);
        }

        TEST_F(DequeTest, FrontAfterPushFront) {
            sample_deque.push_front(5);
            EXPECT_EQ(sample_deque.front(), 5);
        }

        TEST_F(DequeTest, FrontAfterPopFront) {
            sample_deque.pop_front();
            EXPECT_EQ(sample_deque.front(), 20);
        }

        TEST_F(DequeTest, FrontThrowsWhenEmpty) {
            ASSERT_THROW(empty_deque.front(), std::out_of_range);
        }

        // Тесты для back()
        TEST_F(DequeTest, BackReturnsLastElement) {
            EXPECT_EQ(sample_deque.back(), 30);
        }

        TEST_F(DequeTest, BackAfterPushBack) {
            sample_deque.push_back(40);
            EXPECT_EQ(sample_deque.back(), 40);
        }

        TEST_F(DequeTest, BackAfterPopBack) {
            sample_deque.pop_back();
            EXPECT_EQ(sample_deque.back(), 20);
        }

        TEST_F(DequeTest, BackThrowsWhenEmpty) {
            ASSERT_THROW(empty_deque.back(), std::out_of_range);
        }

        // Комбинированные тесты
        TEST_F(DequeTest, FrontAndBackAfterModifications) {
            Deque<int> dq;
            dq.push_front(100);
            EXPECT_EQ(dq.front(), 100);
            EXPECT_EQ(dq.back(), 100);

            dq.push_back(200);
            EXPECT_EQ(dq.front(), 100);
            EXPECT_EQ(dq.back(), 200);
        }

        TEST_F(DequeTest, ConstFrontAndBack) {
            const Deque<int> const_deque{5, 10, 15};
            EXPECT_EQ(const_deque.front(), 5);
            EXPECT_EQ(const_deque.back(), 15);
        }

    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}