#include "solution.h"

int main() {
    Vector vector({1, 2, 3, 4, 5});
    std::cout << vector;
    vector.erase(2);
    std::cout << vector;
    vector.push_back(6);
    std::cout << vector;
    return 0;
}
