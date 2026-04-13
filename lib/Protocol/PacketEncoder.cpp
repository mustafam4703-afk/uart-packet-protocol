#include "PacketEncoder.h"

std::vector<uint8_t> PacketEncoder::encode(uint8_t msgId, const std::vector<uint8_t>& payload) {
    std::vector<uint8_t> returnPacket{};
    returnPacket.push_back(0xAA); // Start byte
    returnPacket.push_back(msgId); // Message ID
    returnPacket.push_back(static_cast<uint8_t>(payload.size())); // Payload length
    returnPacket.insert(returnPacket.end(), payload.begin(), payload.end()); // Payload bytes
    uint8_t checksum = 0;
    for(const auto& byte : payload) {
        checksum ^= byte; // Simple XOR checksum
    }
    returnPacket.push_back(checksum); // Checksum byte
    return returnPacket;
}