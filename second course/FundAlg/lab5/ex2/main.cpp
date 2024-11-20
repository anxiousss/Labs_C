#include "solution.h"

int main() {
    std::vector<std::byte> bytes;

    bytes.push_back(std::byte{0x01});
    bytes.push_back(std::byte{0x02});
    bytes.push_back(std::byte{0x03});

    Encoder encoder(bytes);
    try {
        encoder.encode("input.txt", "out.txt");
    } catch (const stream_error&) {
        std::cout << "File error" << std::endl;
        return 1;
    }
    return 0;
}
