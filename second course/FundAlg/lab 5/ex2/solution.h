
#ifndef EX5_2_SOLUTION_H
#define EX5_2_SOLUTION_H

#include <iostream>
#include <vector>
#include <cstddef>
#include <fstream>
#include <filesystem>

class stream_error: std::exception {};

class equal_paths: std::exception {};

class Encoder {
private:
    std::vector<std::byte> key;
    std::vector<std::byte> s;
    size_t i = 0;
    size_t j = 0;

    void reset();

    void KSA();

    std::byte PRGA();

public:
    Encoder(const std::vector<std::byte>& key);

    void encrypt(const std::string& in_path, const std::string& out_path);

    void decrypt(const std::string& in_path, const std::string& out_path);

    void process(const std::string& in_path, const std::string& out_path);

    void set_key(const std::vector<std::byte>& key);
};

#endif //EX5_2_SOLUTION_H
