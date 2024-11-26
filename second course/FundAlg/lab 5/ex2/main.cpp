#include "solution.h"

int main() {
    std::vector<std::byte> bytes;

    bytes.push_back(std::byte{0x01});
    bytes.push_back(std::byte{0x02});
    bytes.push_back(std::byte{0x03});

    Encoder encoder(bytes);
    try {
        encoder.encrypt("photo_2024-11-26_13-40-14.jpg", "rrrr.txt");
        encoder.decrypt("rrrr.txt", "out.txt");
    } catch (const stream_error&) {
        std::cout << "File error" << std::endl;
        return 1;
    }
    return 0;
}
