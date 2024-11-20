

#include "solution.h"

Encoder::Encoder(const std::vector<std::byte>& key)
        : key(key) {
    KSA();
}

void Encoder::set(const std::vector<std::byte>& key) {
    this->key = key;
    KSA();
}

void Encoder::KSA() {
    s.resize(256);
    for (size_t i = 0; i < 256; ++i) {
        s[i] = static_cast<std::byte>(i);
    }
    size_t j = 0;
    for (size_t i = 0; i < 256; ++i) {
        j = (j + static_cast<size_t>(s[i]) + static_cast<size_t>(key[i % key.size()])) % 256;
        std::swap(s[i], s[j]);
    }
    this->i = 0;
    this->j = 0;
}

std::byte Encoder::PRGA() {
    i = (i + 1) % 256;
    j = (j + static_cast<size_t>(s[i])) % 256;
    std::swap(s[i], s[j]);
    return s[(static_cast<size_t>(s[i]) + static_cast<size_t>(s[j])) % 256];
}

void Encoder::encode(const std::string& in_path, const std::string& out_path) {
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
        std::byte key_byte = PRGA();
        std::byte encrypted_byte = static_cast<std::byte>(c) ^ key_byte;
        out.put(static_cast<char>(encrypted_byte));
    }

    in.close();
    out.close();
}

Encoder::~Encoder() = default;