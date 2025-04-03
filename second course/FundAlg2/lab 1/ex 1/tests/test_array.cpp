#include <gtest/gtest.h>
#include <ranges>
#include <stdexcept>
#include <vector>
#include "../include/array.hpp"

namespace my_container {

    class ArrayTest : public ::testing::Test {
    protected:
        using IntArray5 = Array<int, 5>;
        IntArray5 default_array;
        IntArray5 init_array{1, 2, 3, 4, 5};
        IntArray5 another_array{6, 7, 8, 9, 10};

        void SetUp() override {
            init_array = {1, 2, 3, 4, 5};
        }
    };

    template <typename T, size_t N>
    void test_array_content(const Array<T, N>& arr, const std::vector<T>& expected) {
        ASSERT_EQ(arr.size(), expected.size());
        for (size_t i = 0; i < expected.size(); ++i) {
            EXPECT_EQ(arr[i], expected[i]) << "Mismatch at index " << i;
        }
    }

    TEST_F(ArrayTest, DefaultConstructorInitializesZeroes) {
        test_array_content(default_array, {0, 0, 0, 0, 0});
    }

    TEST_F(ArrayTest, InitializerListConstructorValidSize) {
        test_array_content(init_array, {1, 2, 3, 4, 5});
    }

    TEST_F(ArrayTest, InitializerListConstructorThrowsOnInvalidSize) {
        EXPECT_THROW(
                (Array<int, 5>(std::initializer_list<int>{1, 2})),
                std::invalid_argument
        );

        EXPECT_THROW(
                (Array<int, 5>(std::initializer_list<int>{1, 2, 3, 4, 5, 6})),
                std::invalid_argument
        );
    }

    TEST_F(ArrayTest, CopyConstructorCreatesIndependentCopy) {
        IntArray5 copy(init_array);
        copy[0] = 100;
        test_array_content(init_array, {1, 2, 3, 4, 5});
        test_array_content(copy, {100, 2, 3, 4, 5});
    }

    TEST_F(ArrayTest, MoveConstructorTransfersOwnership) {
        IntArray5 original{1, 2, 3, 4, 5};
        IntArray5 moved(std::move(original));
        test_array_content(moved, {1, 2, 3, 4, 5});
        test_array_content(original, {0, 0, 0, 0, 0});
    }

    TEST_F(ArrayTest, CopyAssignmentOperator) {
        default_array = init_array;
        test_array_content(default_array, {1, 2, 3, 4, 5});

        default_array = default_array;
        test_array_content(default_array, {1, 2, 3, 4, 5});
    }

    TEST_F(ArrayTest, MoveAssignmentOperator) {
        IntArray5 original{1, 2, 3, 4, 5};
        default_array = std::move(original);
        test_array_content(default_array, {1, 2, 3, 4, 5});
        test_array_content(original, {0, 0, 0, 0, 0});
    }
    TEST_F(ArrayTest, InitializerListAssignment) {
        init_array = {5, 4, 3, 2, 1};
        test_array_content(init_array, {5, 4, 3, 2, 1});

        EXPECT_THROW((init_array = {1, 2}), std::invalid_argument);
    }

    TEST_F(ArrayTest, SubscriptOperator) {
        init_array[0] = 10;
        EXPECT_EQ(init_array[0], 10);
        EXPECT_EQ(init_array[4], 5);

        const auto& const_ref = init_array;
        EXPECT_EQ(const_ref[1], 2);
    }

    TEST_F(ArrayTest, AtMethodBoundsChecking) {
        EXPECT_EQ(init_array.at(0), 1);
        EXPECT_THROW(init_array.at(5), std::out_of_range);
        EXPECT_THROW(init_array.at(-1), std::out_of_range);

        const auto& const_ref = init_array;
        EXPECT_EQ(const_ref.at(4), 5);
        EXPECT_THROW(const_ref.at(5), std::out_of_range);
    }

    TEST_F(ArrayTest, FrontAndBackMethods) {
        EXPECT_EQ(init_array.front(), 1);
        EXPECT_EQ(init_array.back(), 5);

        init_array.front() = 10;
        init_array.back() = 20;
        test_array_content(init_array, {10, 2, 3, 4, 20});
    }

    TEST_F(ArrayTest, DataMethodProvidesDirectAccess) {
        IntArray5 arr{10, 20, 30, 40, 50};
        int* data = arr.data();
        data[0] = 100;
        EXPECT_EQ(arr[0], 100);

        const auto& const_ref = arr;
        const int* cdata = const_ref.data();
        EXPECT_EQ(cdata[4], 50);
    }
    TEST_F(ArrayTest, IteratorsCoverFullRange) {
        std::vector<int> result;
        for (int & it : init_array) {
            result.push_back(it);
        }
        EXPECT_EQ(result, std::vector<int>({1, 2, 3, 4, 5}));
    }

    TEST_F(ArrayTest, ReverseIteratorsWorkCorrectly) {
        std::vector<int> reversed;
        for (int & rit : std::ranges::reverse_view(init_array)) {
            reversed.push_back(rit);
        }
        EXPECT_EQ(reversed, std::vector<int>({5, 4, 3, 2, 1}));
    }

    TEST_F(ArrayTest, ConstIterators) {
        const auto& const_ref = init_array;
        std::vector<int> result;
        for (auto it = const_ref.cbegin(); it != const_ref.cend(); ++it) {
            result.push_back(*it);
        }
        EXPECT_EQ(result, std::vector<int>({1, 2, 3, 4, 5}));
    }

    TEST_F(ArrayTest, SizeMethodsReturnCorrectValues) {
        EXPECT_EQ(init_array.size(), 5);
        EXPECT_EQ(init_array.max_size(), 5);
        EXPECT_FALSE(init_array.empty());
    }

    TEST_F(ArrayTest, FillMethodSetsAllElements) {
        init_array.fill(42);
        test_array_content(init_array, {42, 42, 42, 42, 42});
    }

    TEST_F(ArrayTest, SwapExchangesContents) {
        init_array.swap(another_array);
        test_array_content(init_array, {6, 7, 8, 9, 10});
        test_array_content(another_array, {1, 2, 3, 4, 5});
    }

    TEST_F(ArrayTest, EqualityOperators) {
        IntArray5 arr1{1, 2, 3, 4, 5};
        IntArray5 arr2{1, 2, 3, 4, 5};
        IntArray5 arr3{2, 1, 3, 4, 5};

        EXPECT_TRUE(arr1 == arr2);
        EXPECT_FALSE(arr1 == arr3);
        EXPECT_TRUE(arr1 != arr3);
    }

    TEST_F(ArrayTest, ThreeWayComparisonOperator) {
        IntArray5 arr1{1, 2, 3, 4, 5};
        IntArray5 arr2{1, 2, 3, 4, 6};
        IntArray5 arr3{1, 2, 3, 3, 5};

        EXPECT_EQ(arr1 <=> arr1, std::strong_ordering::equal);
        EXPECT_EQ(arr1 <=> arr2, std::strong_ordering::less);
        EXPECT_EQ(arr1 <=> arr3, std::strong_ordering::greater);
    }

    TEST_F(ArrayTest, NoexceptSpecifiers) {
        EXPECT_TRUE(noexcept(Array<int, 5>()));
        EXPECT_TRUE(noexcept(init_array.swap(another_array)));
        EXPECT_TRUE(noexcept(init_array.data()));
        EXPECT_TRUE(noexcept(init_array.size()));
        EXPECT_TRUE(noexcept(init_array.empty()));
    }

}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}