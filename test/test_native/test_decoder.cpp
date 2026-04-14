#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "IUARTBus.h"
#include "PacketDecoder.h"
#include "PacketEncoder.h"

class MockUARTBus : public IUARTBus {
public:
    MOCK_METHOD(void, sendBytes, (const std::vector<uint8_t>& bytes), (override));
    MOCK_METHOD(std::vector<uint8_t>, readBytes, (size_t count), (override));
};

TEST(PacketDecoderTest, ExtractMessageId)
{
    PacketDecoder decoder;
    const uint8_t StartByte = 0xAA; // Valid start byte
    const uint8_t expectedMsgId = 0x01;
    const std::vector<uint8_t> payLoad = {0x00, 0x00, 0x00};
    std::vector<uint8_t> encodedPacket = {StartByte, expectedMsgId, static_cast<uint8_t>(payLoad.size())};
    encodedPacket.insert(encodedPacket.end(), payLoad.begin(), payLoad.end()); 
    encodedPacket.push_back(calculateChecksum(payLoad)); // Add checksum

    auto decodedPacket = decoder.decode(encodedPacket);
    ASSERT_EQ(decodedPacket.messageId, expectedMsgId);
    //ASSERT_TRUE(true);
}

TEST(PacketDecoderTest, IsValidIsTrueWhenStartByteIsValid)
{
    PacketDecoder decoder;
    const uint8_t StartByte = 0xAA; // Valid start byte
    const uint8_t MsgId = 0x01;
    const std::vector<uint8_t> payLoad = {0x00, 0x00, 0x00};
    std::vector<uint8_t> encodedPacket = {StartByte, MsgId, static_cast<uint8_t>(payLoad.size())};
    encodedPacket.insert(encodedPacket.end(), payLoad.begin(), payLoad.end()); 
    encodedPacket.push_back(calculateChecksum(payLoad)); // Add checksum

    auto decodedPacket = decoder.decode(encodedPacket);
    const auto expectedIsValid = PacketStatus::VALID_PACKET;
    ASSERT_EQ(decodedPacket.status, expectedIsValid);
}

TEST(PacketDecoderTest, IsValidIsFalseWhenStartByteIsInvalid)
{
    PacketDecoder decoder;
    uint8_t StartByte = 0xAB; // Invalid start byte
    const uint8_t MsgId = 0x01;
    const std::vector<uint8_t> payLoad = {0x00, 0x00, 0x00};
    std::vector<uint8_t> encodedPacket = {StartByte, MsgId, static_cast<uint8_t>(payLoad.size())};
    encodedPacket.insert(encodedPacket.end(), payLoad.begin(), payLoad.end());
    encodedPacket.push_back(calculateChecksum(payLoad)); // Add checksum 

    auto decodedPacket = decoder.decode(encodedPacket);
    const auto expectedIsValid = PacketStatus::INVALID_PACKET;
    ASSERT_EQ(decodedPacket.status, expectedIsValid);
}

TEST(PacketDecoderTest, PayloadIsExtractedCorrectly)
{
    PacketDecoder decoder;
    uint8_t StartByte = 0xAA; // Valid start byte
    const uint8_t MsgId = 0x01;
    const std::vector<uint8_t> payLoad = {0x01, 0x02, 0x03, 0x04};
    std::vector<uint8_t> encodedPacket = {StartByte, MsgId, static_cast<uint8_t>(payLoad.size())};
    encodedPacket.insert(encodedPacket.end(), payLoad.begin(), payLoad.end());
    encodedPacket.push_back(calculateChecksum(payLoad)); // Add checksum 

    auto decodedPacket = decoder.decode(encodedPacket);
    const auto expectedExtractedPayload = payLoad;
    ASSERT_EQ(decodedPacket.payload, expectedExtractedPayload);
}

TEST(PacketDecoderTest, InvalidPacketHasInvalidChecksum)
{
    PacketDecoder decoder;
    uint8_t StartByte = 0xAA; // Valid start byte
    const uint8_t MsgId = 0x01;
    const std::vector<uint8_t> payLoad = {0x01, 0x02, 0x03, 0x04};
    std::vector<uint8_t> encodedPacket = {StartByte, MsgId, static_cast<uint8_t>(payLoad.size())};
    encodedPacket.insert(encodedPacket.end(), payLoad.begin(), payLoad.end());
    const uint8_t invalidChecksum = calculateChecksum(payLoad) + 1; // Intentionally make it invalid
    encodedPacket.push_back(invalidChecksum); // Add invalid checksum

    auto decodedPacket = decoder.decode(encodedPacket);
    const auto expectedPacketInvalid = PacketStatus::INVALID_PACKET;
    ASSERT_EQ(decodedPacket.status, expectedPacketInvalid);
}

TEST(PacketDecoderTest, PacketBelowMinimumPacketSizeIsInvalid)
{
    PacketDecoder decoder;
    uint8_t StartByte = 0xAA; // Valid start byte
    const uint8_t MsgId = 0x01;
    const std::vector<uint8_t> payLoad = {}; // Empty payload
    std::vector<uint8_t> encodedPacket = {StartByte, 0x00, 0x00}; // Missing message ID and payload length, and no checksum

    auto decodedPacket = decoder.decode(encodedPacket);
    const auto expectedPacketInvalid = PacketStatus::INVALID_PACKET;
    ASSERT_EQ(decodedPacket.status, expectedPacketInvalid);
}