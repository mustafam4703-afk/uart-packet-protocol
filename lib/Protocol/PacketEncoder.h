#ifndef PACKETENCODER_H
#define PACKETENCODER_H

#include "ChecksumCalculator.h"
#include <cstdint>
#include <vector>

class PacketEncoder {
public:
    std::vector<uint8_t> encode(uint8_t msgId, const std::vector<uint8_t>& payload);
};

#endif // PACKETENCODER_H