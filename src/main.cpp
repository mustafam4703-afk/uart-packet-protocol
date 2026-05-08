#include <Arduino.h>
#include <HardwareSerial.h>
#include "PacketEncoder.h"
#include "PacketDecoder.h"
#include "RealUARTBus.h"

enum class BoardRole {
    SENDER_BOARD,
    RECEIVER_BOARD
};

const BoardRole role = BoardRole::RECEIVER_BOARD; // Change to SENDER for the sending board

const int TX_PIN = 17;
const int RX_PIN = 18;

HardwareSerial mySerial(2); // UART2
RealUARTBus uart(mySerial);
PacketEncoder encoder;
PacketDecoder decoder;

void executeSenderLogic() {
    std::vector<uint8_t> payload = {0x01, 0x02, 0x03};
    auto packet = encoder.encode(0x01, payload);
    uart.sendBytes(packet);
    Serial.println("Packet sent.");
    delay(1000);
}

void executeReceiverLogic() {
    auto raw = uart.readBytes(7);
    if (!raw.empty() && raw[0] == 0xAA && raw.size() == 7) { // Check for start byte
        auto result = decoder.decode(raw);
        Serial.print("Raw bytes: ");
        for (auto b : raw) {
            Serial.print(b, HEX);
            Serial.print(" ");
        }
        Serial.println();
        if (result.status == PacketStatus::VALID_PACKET) {
            Serial.println("Valid packet received!");
            Serial.print("Message ID: ");
            Serial.println(result.messageId);
        } else {
            Serial.println("Invalid packet.");
        }
    }
}

void setup() {
    Serial.begin(115200);
    mySerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
    Serial.println("Ready.");
}

void loop() {
    role == BoardRole::SENDER_BOARD ? executeSenderLogic() : executeReceiverLogic();
}