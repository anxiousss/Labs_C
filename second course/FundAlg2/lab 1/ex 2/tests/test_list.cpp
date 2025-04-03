#include <gtest/gtest.h>
#include "../include/List.hpp"

namespace my_container {
    namespace {

        class ListTest : public ::testing::Test {
        protected:
            List<int> empty_list;
            List<int> sample_list;

            ListTest() : empty_list(), sample_list({1, 2, 3}) {}

            void SetUp() override {}
        };

        TEST_F(ListTest, DefaultConstructor) {
            EXPECT_TRUE(empty_list.empty());
            EXPECT_EQ(empty_list.size(), 0);
        }

        TEST_F(ListTest, InitializerListConstructor) {
            EXPECT_EQ(sample_list.size(), 3);
            EXPECT_EQ(sample_list.front(), 1);
            EXPECT_EQ(sample_list.back(), 3);
        }

        TEST_F(ListTest, CopyConstructor) {
            List<int> copy(sample_list);
            EXPECT_EQ(copy.size(), 3);
            EXPECT_EQ(copy, sample_list);
        }

        TEST_F(ListTest, MoveConstructor) {
            List<int> moved(std::move(sample_list));
            EXPECT_EQ(moved.size(), 3);
            EXPECT_TRUE(sample_list.empty());
        }

        TEST_F(ListTest, CopyAssignment) {
            List<int> copy;
            copy = sample_list;
            EXPECT_EQ(copy.size(), 3);
            EXPECT_EQ(copy, sample_list);
        }

        TEST_F(ListTest, MoveAssignment) {
            List<int> moved;
            moved = std::move(sample_list);
            EXPECT_EQ(moved.size(), 3);
            EXPECT_TRUE(sample_list.empty());
        }

        TEST_F(ListTest, FrontBack) {
            EXPECT_EQ(sample_list.front(), 1);
            EXPECT_EQ(sample_list.back(), 3);
        }

        TEST_F(ListTest, FrontBackThrowWhenEmpty) {
            EXPECT_THROW(empty_list.front(), std::out_of_range);
            EXPECT_THROW(empty_list.back(), std::out_of_range);
        }

        TEST_F(ListTest, Iterators) {
            auto it = sample_list.begin();
            EXPECT_EQ(*it, 1);
            ++it;
            EXPECT_EQ(*it, 2);
            ++it;
            EXPECT_EQ(*it, 3);
            ++it;
            EXPECT_EQ(it, sample_list.end());
        }

        TEST_F(ListTest, InsertErase) {
            auto it = sample_list.begin();
            ++it;
            sample_list.insert(it, 99);
            List<int> expected{1, 99, 2, 3};
            EXPECT_EQ(sample_list, expected);

            it = sample_list.begin();
            ++it;
            sample_list.erase(it);
            expected = {1, 2, 3};
            EXPECT_EQ(sample_list, expected);
        }

        TEST_F(ListTest, Insert) {
            auto it = sample_list.begin();
            ++it;
            sample_list.insert(it, 99);

            my_container::List<int> expected{1, 99, 2, 3};
            EXPECT_EQ(sample_list, expected);
        }

        TEST_F(ListTest, Resize) {
            sample_list.resize(5);
            EXPECT_EQ(sample_list.size(), 5);
            EXPECT_EQ(sample_list.back(), 0);

            sample_list.resize(2);
            EXPECT_EQ(sample_list.size(), 2);
            EXPECT_EQ(sample_list.back(), 2);
        }

        TEST_F(ListTest, Swap) {
            List<int> other{4, 5, 6};
            sample_list.swap(other);
            EXPECT_EQ(sample_list.size(), 3);
            EXPECT_EQ(sample_list.back(), 6);
            EXPECT_EQ(other.size(), 3);
            EXPECT_EQ(other.back(), 3);
        }

        TEST_F(ListTest, Comparisons) {
            List<int> list_eq{1, 2, 3};
            List<int> list_less{1, 2};
            List<int> list_greater{1, 2, 4};

            EXPECT_EQ(sample_list, list_eq);
            EXPECT_NE(sample_list, list_less);
            EXPECT_LT(list_less, sample_list);
        }

        TEST_F(ListTest, MaxSize) {
            EXPECT_EQ(empty_list.max_size(), std::numeric_limits<size_t>::max());
            EXPECT_GT(empty_list.max_size(), 0);

            EXPECT_EQ(sample_list.max_size(), std::numeric_limits<size_t>::max());

            my_container::List<int> another_list{4, 5, 6};
            EXPECT_EQ(empty_list.max_size(), another_list.max_size());

            static_assert(noexcept(empty_list.max_size()),
                          "max_size() should be noexcept");
        }

    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}