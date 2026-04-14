#include "PacketDecoder.h"

DecodedPacket PacketDecoder::decode(const std::vector<uint8_t>& packet) {
    DecodedPacket decodedPacket{};
    const size_t MIN_PACKET_SIZE = 4; // Minimum packet size: Start byte + Message ID + Payload Length + Checksum
    const bool firstByteIsStartByte = !packet.empty() && packet[0] == 0xAA; // Check if the first byte is the start byte
    if((packet.size() >= MIN_PACKET_SIZE) && firstByteIsStartByte) {
        decodedPacket.messageId = packet[1]; // Assuming the message ID is always at index 1
        const auto PAYLOAD_START_INDEX = 3;
        const auto PAYLOAD_END_INDEX = packet.end() - 1; // Assuming the last byte is the checksum
        const auto PAYLOAD_LENGTH = packet[2]; // Assuming the payload length is always at index 2
        decodedPacket.payload = std::vector<uint8_t>(packet.begin() + PAYLOAD_START_INDEX, PAYLOAD_END_INDEX); // Assuming payload starts from index 3
        const bool payloadLengthMatches = decodedPacket.payload.size() == PAYLOAD_LENGTH; // Check if the payload length matches the expected length

        decodedPacket.status = (firstByteIsStartByte && payloadLengthMatches && packet.back() == calculateChecksum(decodedPacket.payload)) ? PacketStatus::VALID_PACKET : PacketStatus::INVALID_PACKET; // Check if the first byte is the start byte and if the payload length matches and if the checksum is correct
    } else {
        decodedPacket.status = PacketStatus::INVALID_PACKET; // If the first byte is not the start byte, mark it as an invalid packet
    }
    
    return decodedPacket;
}