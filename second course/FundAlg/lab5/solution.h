
#ifndef EX5_1_SOLUTION_H
#define EX5_1_SOLUTION_H


#include <iostream>
#include <utility>

class binary_int {
private:
public:
    int number;
    binary_int();
    binary_int(int a);
    binary_int operator- () const;
    binary_int operator+ (binary_int x) const;
    binary_int operator- (binary_int x) const;
    binary_int &operator+= (binary_int x);
    binary_int &operator-= (binary_int x);
    binary_int &operator++ ();
    binary_int &operator-- ();
    bool operator< (binary_int x) const;
    binary_int &operator*= (binary_int x);
    binary_int operator* (binary_int x) const;
    binary_int operator<< (binary_int x) const;
    binary_int operator>> (binary_int x) const;
    binary_int &operator<<= (binary_int x);
    binary_int &operator>>= (binary_int x);
    binary_int operator& (binary_int) const;

    std::pair<binary_int, binary_int> split() const;
};

std::ostream& operator<<(std::ostream &stream, binary_int binaryInt);

int add(int a, int b);

#endif //EX5_1_SOLUTION_H