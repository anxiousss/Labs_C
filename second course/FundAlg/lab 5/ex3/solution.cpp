
#include "solution.h"

LogicalValuesArray::LogicalValuesArray(unsigned int _value) {
    this->_value = _value;
}

unsigned int LogicalValuesArray::get() const {
    return this->_value;
}

bool LogicalValuesArray::equals(const LogicalValuesArray &n) const {
    return this->_value == n.get();
}

bool LogicalValuesArray::get_bit(unsigned int index) const {
    return (this->_value & (1 << (sizeof(unsigned int) * 8 - index - 1)));
}

LogicalValuesArray LogicalValuesArray::operator~() const {
    return {~this->_value};
}

LogicalValuesArray LogicalValuesArray::operator&(const LogicalValuesArray &n) const {
    return this->_value & n.get();
}

LogicalValuesArray LogicalValuesArray::operator|(const LogicalValuesArray &n) const {
    return ~LogicalValuesArray(~(*this) & ~(n));
}

LogicalValuesArray LogicalValuesArray::operator^(const LogicalValuesArray &n) const {
    return LogicalValuesArray(((~(*this) & n) | (*this & (~n))));
}

LogicalValuesArray LogicalValuesArray::operator<=(const LogicalValuesArray &n) const {
    return ~(*this) | n;
}

LogicalValuesArray LogicalValuesArray::operator>(const LogicalValuesArray &n) const {
    return ~(*this <= n);
}

LogicalValuesArray LogicalValuesArray::operator==(const LogicalValuesArray &n) const {
    return LogicalValuesArray(((~(*this) | n) & (*this | (~n))));
}

LogicalValuesArray LogicalValuesArray::pierce_arrow(const LogicalValuesArray &n) const {
    return LogicalValuesArray(~(*this | n));
}

LogicalValuesArray LogicalValuesArray::sheffer_stroke(const LogicalValuesArray &n) const {
    return LogicalValuesArray(~(*this & n));
}

void LogicalValuesArray::to_string(char *result) {
    std::memset(result, '\0', sizeof(unsigned int) * 8);
    for (int i = sizeof(unsigned int) * 8 - 1, j = 0; i >= 0; --i, j++) {
        result[j] = (char)((char)((this->_value >> i) & 1) + '0');
    }
}

std::ostream& operator<<(std::ostream& out, const LogicalValuesArray& el) {
    for (int i = sizeof(unsigned int) * 8 - 1; i >= 0; --i) {
        out << ((el.get() >> i) & 1);
    }
    return out;
}





