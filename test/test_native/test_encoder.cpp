#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "IUARTBus.h"
#include "PacketEncoder.h"
#include <numeric>

class MockUARTBus : public IUARTBus {
public:
    MOCK_METHOD(void, sendBytes, (const std::vector<uint8_t>& bytes), (override));
    MOCK_METHOD(std::vector<uint8_t>, readBytes, (size_t count), (override));
};

TEST(PacketEncoderTest, FirstByteIsStartByte)
{
    PacketEncoder encoder;
    auto packet = encoder.encode(0x01, {0x42});  
    ASSERT_EQ(packet[0], 0xAA);
}

TEST(PacketEncoderTest, SecondByteIsMessageId)
{
    PacketEncoder encoder;
    const uint8_t msgId = 0x01;
    const std::vector<uint8_t> payload = {0x42};
    auto packet = encoder.encode(msgId, payload);
    ASSERT_EQ(packet[1], msgId);
}

TEST(PacketEncoderTest, ThirdByteIsPayloadLength)
{
    PacketEncoder encoder;
    const uint8_t msgId = 0x01;
    const std::vector<uint8_t> payload = {0x42, 0x76, 0x89};
    const uint8_t expectedPayloadLength = payload.size();
    auto packet = encoder.encode(msgId, payload);
    ASSERT_EQ(packet[2], expectedPayloadLength);
}

TEST(PacketEncoderTest, PayloadBytesIncludedAfterLengthByte)
{
    PacketEncoder encoder;
    const uint8_t msgId = 0x01;
    const std::vector<uint8_t> payload = {0x42, 0x76, 0x89};
    const uint8_t expectedPayloadLength = payload.size();
    const uint8_t PAYLOAD_START_INDEX = 3;
    auto packet = encoder.encode(msgId, payload);
    ASSERT_EQ(std::vector<uint8_t>(packet.begin() + PAYLOAD_START_INDEX, packet.end() - 1), payload);
}

TEST(PacketEncoderTest, CheckSumByte)
{
    PacketEncoder encoder;
    const uint8_t msgId = 0x01;
    const std::vector<uint8_t> payload = {0x42, 0x76, 0x89};
    uint8_t EXPECTED_CHECKSUM_BYTE{};
    for(const auto& byte : payload) {
        EXPECTED_CHECKSUM_BYTE ^= byte;
    }
    auto packet = encoder.encode(msgId, payload);
    ASSERT_EQ(packet.back(), EXPECTED_CHECKSUM_BYTE);
}

