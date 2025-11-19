#include "../include/fixed_block_resource.hpp"
#include "../include/pmr_stack.hpp"

#include <iostream>
#include <string>

struct Point {
    int x;
    int y;
    double z;
    std::string name;

    friend std::ostream& operator<<(std::ostream& os, const Point& p) {
        os << "Point(" << p.x << ", " << p.y << ", " << p.z << ", \"" << p.name << "\")";
        return os;
    }
};

int main() {
    constexpr std::size_t memory_size = 4096; 
    FixedBlockMemoryResource memory_resource(memory_size);

    std::cout << "=== Демонстрация с int ===\n";
    {
        Pmr_stack<int> stack(&memory_resource);

        for (int i = 1; i <= 5; ++i) {
            stack.push(i * 10);
        }

        std::cout << "Размер стека: " << stack.size() << "\n";
        std::cout << "Вершина: " << stack.top() << "\n";

        std::cout << "Элементы: ";
        for (auto it = stack.begin(); it != stack.end(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << "\n";

        stack.pop();
        stack.pop();
        std::cout << "После pop вершина: " << stack.top() << "\n";
    } 

    std::cout << "\n=== Демонстрация с Point ===\n";
    {
        Pmr_stack<Point> stack(&memory_resource);

        stack.push(Point{1, 2, 3.5, "A"});
        stack.push(Point{4, 5, 6.0, "B"});
        stack.push(Point{7, 8, 9.1, "C"});

        std::cout << "Размер стека: " << stack.size() << "\n";
        std::cout << "Вершина: " << stack.top() << "\n";

        std::cout << "Все элементы:\n";
        for (const auto& p : stack) { 
            std::cout << "  " << p << "\n";
        }

        stack.pop();
        stack.push(Point{0, 0, 0.0, "REUSED"});
        for (const auto& p : stack) {
            std::cout << "  " << p << "\n";
        }
    }

    std::cout << "\n=== Память освобождена  ===\n";
    return 0;
}