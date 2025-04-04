#include <gtest/gtest.h>

#include "../include/vector.hpp"

namespace my_container::test {
    class VectorTest : public ::testing::Test {
    protected:
        void SetUp() override {
            vec_int = {1, 2, 3};
            vec_str = {"a", "b", "c"};
        }

        Vector<int> vec_int;
        Vector<std::string> vec_str;
        Vector<int> empty_vec;
    };

    TEST_F(VectorTest, DefaultConstructor) {
        EXPECT_EQ(empty_vec.size(), 0);
        EXPECT_EQ(empty_vec.capacity(), 0); // Было 4, должно быть 0
    }

    TEST_F(VectorTest, CopyConstructor) {
        Vector<int> copy(vec_int);
        EXPECT_EQ(copy.size(), 3);
        EXPECT_EQ(copy[0], 1);
    }

    TEST_F(VectorTest, MoveConstructor) {
        Vector<int> moved(std::move(vec_int));
        EXPECT_EQ(moved.size(), 3);
        EXPECT_TRUE(vec_int.empty());
    }

    TEST_F(VectorTest, InitializerListConstructor) {
        Vector init_list{1, 2, 3};
        EXPECT_EQ(init_list.size(), 3);
    }

    TEST_F(VectorTest, CopyAssignment) {
        Vector<int> copy;
        copy = vec_int;
        EXPECT_EQ(copy.size(), 3);
        EXPECT_EQ(copy[1], 2);
    }

    TEST_F(VectorTest, MoveAssignment) {
        Vector<int> moved;
        moved = std::move(vec_int);
        EXPECT_EQ(moved.size(), 3);
        EXPECT_TRUE(vec_int.empty());
    }

    TEST_F(VectorTest, ElementAccess) {
        EXPECT_EQ(vec_int[0], 1);
        vec_int[1] = 42;
        EXPECT_EQ(vec_int[1], 42);
        EXPECT_EQ(vec_int.at(2), 3);
        EXPECT_THROW(vec_int.at(5), std::out_of_range);
    }

    TEST_F(VectorTest, FrontBackData) {
        EXPECT_EQ(vec_int.front(), 1);
        EXPECT_EQ(vec_int.back(), 3);
        EXPECT_EQ(*vec_int.data(), 1);
    }

    TEST_F(VectorTest, CapacityMethods) {
        EXPECT_FALSE(vec_int.empty());
        vec_int.reserve(10);
        EXPECT_GE(vec_int.capacity(), 10);
        vec_int.shrink_to_fit();
        EXPECT_EQ(vec_int.capacity(), 3);
    }

    TEST_F(VectorTest, PushPop) {
        vec_int.push_back(4);
        EXPECT_EQ(vec_int.size(), 4);
        vec_int.pop_back();
        EXPECT_EQ(vec_int.size(), 3);
    }

    TEST_F(VectorTest, InsertErase) {
        vec_int.insert(vec_int.begin() + 1, 42);
        EXPECT_EQ(vec_int[1], 42);
        EXPECT_EQ(vec_int.size(), 4);
        vec_int.erase(vec_int.begin());
        EXPECT_EQ(vec_int[0], 42);
        EXPECT_EQ(vec_int.size(), 3);
    }

    TEST_F(VectorTest, ClearResize) {
        vec_int.clear();
        EXPECT_TRUE(vec_int.empty());
        vec_int.resize(5, 42);
        EXPECT_EQ(vec_int.size(), 5);
        EXPECT_EQ(vec_int[4], 42);
    }
    TEST_F(VectorTest, Comparisons) {
        Vector<int> v1{1, 2, 3};
        Vector<int> v2{1, 2, 4};
        Vector<int> v3{1, 2};
        Vector<int> v4{1, 2, 3};

        EXPECT_TRUE(v1 == v4);
        EXPECT_TRUE(v1 != v2);
        EXPECT_TRUE(v1 < v2);
        EXPECT_TRUE(v3 <= v1);
        EXPECT_TRUE(v2 > v1);
        EXPECT_TRUE(v1 >= v3);

        // Тест с пустыми векторами
        Vector<int> empty1, empty2;
                EXPECT_TRUE(empty1 == empty2);
        EXPECT_FALSE(empty1 < empty2);
    }

    TEST_F(VectorTest, NotEqualOperator) {
        Vector<int> v1{1, 2, 3};
        Vector<int> v2{1, 2, 4};
        Vector<int> v3{1, 2};
        Vector<int> v4{1, 2, 3};

        EXPECT_TRUE(v1 != v2);  // Разные элементы
        EXPECT_TRUE(v1 != v3);  // Разные размеры
        EXPECT_FALSE(v1 != v4); // Идентичные векторы

        // Сравнение с другим типом контейнера
        class DummyContainer : public Container<int> {
        public:
            // Реализация чисто виртуальных методов
            bool operator==(const Container<int>&) const override { return false; }
            bool operator!=(const Container<int>&) const override { return true; }
            size_t size() const override { return 0; }
            size_t max_size() const noexcept override { return 0; }
            bool empty() const override { return true; }
        };
        const DummyContainer dummy;
        EXPECT_TRUE(v1.operator!=(dummy));
    }

    TEST_F(VectorTest, SwapTest) {
        Vector<int> v1{1, 2};
        Vector<int> v2{3, 4, 5};
        v1.swap(v2);
        EXPECT_EQ(v1.size(), 3);
        EXPECT_EQ(v2.size(), 2);
    }

    TEST_F(VectorTest, Iterators) {
        auto it = vec_int.begin();
        EXPECT_EQ(*it, 1);
        EXPECT_EQ(*(vec_int.end() - 1), 3);
        const auto& const_vec = vec_int;
        EXPECT_EQ(*const_vec.cbegin(), 1);
    }

    TEST_F(VectorTest, MoveSemantics) {
        Vector<std::string> moved(std::move(vec_str));
        EXPECT_TRUE(vec_str.empty());
        EXPECT_EQ(moved[0], "a");
    }

    TEST_F(VectorTest, ShrinkToFit) {
        vec_int.reserve(10);
        EXPECT_GE(vec_int.capacity(), 10);

        vec_int.shrink_to_fit();
        EXPECT_EQ(vec_int.capacity(), vec_int.size()); // Универсальная проверка
    }

}  // namespace my_container::test