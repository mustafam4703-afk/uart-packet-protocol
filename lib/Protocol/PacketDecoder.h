#ifndef PACKET_DECODER_H
#define PACKET_DECODER_H

#include "ChecksumCalculator.h"
#include <cstdint>
#include <vector>

enum class PacketStatus: uint8_t {
    NO_PACKET_RECEIVED = 0x00,
    VALID_PACKET = 0x01,
    INVALID_PACKET = 0x02,
    // Add more status codes as needed
};

struct DecodedPacket {
    PacketStatus status{ PacketStatus::NO_PACKET_RECEIVED }; // Default to NO_PACKET_RECEIVED
    uint8_t messageId{};
    std::vector<uint8_t> payload{};
};

class PacketDecoder {
public:
    DecodedPacket decode(const std::vector<uint8_t>& packet);
};

#endif // PACKET_DECODER_H