#ifndef CHECKSUM_CALCULATOR_H
#define CHECKSUM_CALCULATOR_H

#include <cstdint>
#include <vector>

inline uint8_t calculateChecksum(const std::vector<uint8_t>& payload) {
    uint8_t checksum{};
    for (const auto& byte : payload) {
        checksum ^= byte;
    }
    return checksum;
}

#endif // CHECKSUM_CALCULATOR_H