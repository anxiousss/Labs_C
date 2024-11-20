
#ifndef EX5_2_SOLUTION_H
#define EX5_2_SOLUTION_H

#include <iostream>
#include <vector>
#include <cstddef>
#include <fstream>

class stream_error: std::exception {};

class Encoder {
private:
    std::vector<std::byte> key;
    std::vector<std::byte> s; // RC4 permutation vector
    size_t i = 0;             // RC4 indices
    size_t j = 0;

    void KSA();

    std::byte PRGA();

public:
    Encoder(const std::vector<std::byte>& key);
    ~Encoder();

    void encode(const std::string& in_path, const std::string& out_path);
    void set(const std::vector<std::byte>& key);
};

#endif //EX5_2_SOLUTION_H
