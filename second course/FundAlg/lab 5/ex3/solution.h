

#ifndef EX5_3_SOLUTION_H
#define EX5_3_SOLUTION_H

#include <iostream>
#include <cstring>
#include <cassert>
#include <sstream>

class LogicalValuesArray {
private:
    unsigned int _value;
public:
    LogicalValuesArray(unsigned int _value = 0);

    unsigned int get() const;

    bool equals(const LogicalValuesArray& n) const;

    bool get_bit(unsigned int index) const;

    void to_string(char* result);

    LogicalValuesArray operator~() const;

    LogicalValuesArray operator|(const LogicalValuesArray& n) const;

    LogicalValuesArray operator&(const LogicalValuesArray& n) const;

    LogicalValuesArray operator^(const LogicalValuesArray& n) const;

    LogicalValuesArray operator<=(const LogicalValuesArray& n) const;

    LogicalValuesArray operator>(const LogicalValuesArray& n) const;

    LogicalValuesArray operator==(const LogicalValuesArray& n) const;

    LogicalValuesArray pierce_arrow(const LogicalValuesArray& n) const;

    LogicalValuesArray sheffer_stroke(const LogicalValuesArray& n) const;


};

std::ostream& operator<<(std::ostream& out, const LogicalValuesArray& el);


#endif //EX5_3_SOLUTION_H
