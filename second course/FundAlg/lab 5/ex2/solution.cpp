#include "solution.h"

Encoder::Encoder(const std::vector<std::byte>& key)
        : key(key) {
    KSA();
}

void Encoder::set_key(const std::vector<std::byte>& new_key) {
    this->key = new_key;
    reset();
}

void Encoder::encrypt(const std::string& in_path, const std::string& out_path) {
    this->reset();
    process(in_path, out_path);
}

void Encoder::decrypt(const std::string& in_path, const std::string& out_path) {
    encrypt(in_path, out_path);
}

void Encoder::reset() {
    KSA();
}

void Encoder::KSA() {
    this->s.resize(256);
    for (size_t k = 0; k < 256; ++k) {
        this->s[k] = static_cast<std::byte>(k);
    }
    size_t l = 0;
    for (size_t k = 0; k < 256; ++k) {
        l = (l + static_cast<size_t>(this->s[k]) + static_cast<size_t>(key[k % key.size()])) % 256;
        std::swap(this->s[k], this->s[l]);
    }
    this->i = 0;
    this->j = 0;
}

std::byte Encoder::PRGA() {
    this->i = (this->i + 1) % 256;
    this->j = (this->j + static_cast<size_t>(this->s[this->i])) % 256;
    std::swap(this->s[this->i], this->s[this->j]);
    return s[(static_cast<size_t>(this->s[this->i]) + static_cast<size_t>(this->s[this->j])) % 256];
}

void Encoder::process(const std::string& in_path, const std::string& out_path) {
    if (std::filesystem::equivalent(in_path, out_path)) {
        throw equal_paths();
    }

    std::ifstream in(in_path, std::ios::binary);
    if (!in.is_open()) {
        throw stream_error();
    }
    std::ofstream out(out_path, std::ios::out | std::ios::binary);
    if (!out.is_open()) {
        in.close();
        throw stream_error();
    }

    char c;
    while (in.get(c)) {
        std::byte key_byte = this->PRGA();
        std::byte processed_byte = static_cast<std::byte>(c) ^ key_byte;
        out.put(static_cast<char>(processed_byte));
    }

    in.close();
    out.close();
}
