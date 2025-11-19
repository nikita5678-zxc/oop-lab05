#include <gtest/gtest.h>
#include <string>
#include <vector>

#include "../include/fixed_block_resource.hpp"
#include "../include/pmr_stack.hpp"

struct TestData {
    int id;
    std::string name;

    TestData(int i, const std::string& n) : id(i), name(n) {}
    TestData(const TestData& other) : id(other.id), name(other.name) {}
    TestData(TestData&& other) noexcept : id(other.id), name(std::move(other.name)) {}

    bool operator==(const TestData& other) const {
        return id == other.id && name == other.name;
    }
};

std::ostream& operator<<(std::ostream& os, const TestData& d) {
    return os << "TestData(" << d.id << ", \"" << d.name << "\")";
}

// Тест 1: базовые операции со стеком (int)
TEST(PmrStackTest, BasicIntOperations) {
    FixedBlockMemoryResource mr(1024);
    Pmr_stack<int> stack(&mr);

    EXPECT_TRUE(stack.empty());
    EXPECT_EQ(stack.size(), 0);

    stack.push(42);
    EXPECT_FALSE(stack.empty());
    EXPECT_EQ(stack.size(), 1);
    EXPECT_EQ(stack.top(), 42);

    stack.push(100);
    EXPECT_EQ(stack.size(), 2);
    EXPECT_EQ(stack.top(), 100);

    stack.pop();
    EXPECT_EQ(stack.size(), 1);
    EXPECT_EQ(stack.top(), 42);

    stack.pop();
    EXPECT_TRUE(stack.empty());
}

// Тест 2: работа с нетривиальным типом
TEST(PmrStackTest, ComplexTypeOperations) {
    FixedBlockMemoryResource mr(2048);
    Pmr_stack<TestData> stack(&mr);

    stack.push(TestData(1, "first"));
    stack.push(TestData(2, "second"));

    ASSERT_EQ(stack.size(), 2);
    EXPECT_EQ(stack.top(), TestData(2, "second"));

    stack.pop();
    EXPECT_EQ(stack.size(), 1);
    EXPECT_EQ(stack.top(), TestData(1, "first"));
}

// Тест 3: итераторы (forward iterator)
TEST(PmrStackTest, IteratorTraversal) {
    FixedBlockMemoryResource mr(1024);
    Pmr_stack<int> stack(&mr);

    const std::vector<int> expected = {10, 20, 30};
    for (int x : expected) {
        stack.push(x);
    }

    std::vector<int> actual;
    for (auto it = stack.begin(); it != stack.end(); ++it) {
        actual.push_back(*it);
    }

    EXPECT_EQ(actual, expected);
}

// Тест 4: const итераторы
TEST(PmrStackTest, ConstIterator) {
    FixedBlockMemoryResource mr(1024);
    const Pmr_stack<int> stack(&mr); 

    Pmr_stack<int> mutable_stack(&mr);
    mutable_stack.push(1);
    mutable_stack.push(2);

    const auto& const_stack = mutable_stack;
    std::vector<int> actual;
    for (auto it = const_stack.begin(); it != const_stack.end(); ++it) {
        actual.push_back(*it);
    }
    EXPECT_EQ(actual, std::vector<int>({1, 2}));
}

// Тест 5: переиспользование памяти (косвенно)
TEST(PmrStackTest, MemoryReuse) {
    FixedBlockMemoryResource mr(256);
    Pmr_stack<std::string> stack(&mr);

    stack.push("hello");
    stack.push("world");

    stack.pop();

    EXPECT_NO_THROW(stack.push("reuse"));
    EXPECT_EQ(stack.size(), 2);
    EXPECT_EQ(stack.top(), "reuse");
}



// Тест 8: do_is_equal
TEST(FixedBlockMemoryResourceTest, IsEqual) {
    FixedBlockMemoryResource mr1(128);
    FixedBlockMemoryResource mr2(128);

    EXPECT_TRUE(mr1.is_equal(mr1));
    EXPECT_FALSE(mr1.is_equal(mr2));
}