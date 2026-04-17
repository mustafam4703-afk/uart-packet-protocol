#include <Arduino.h>
#include <HardwareSerial.h>
#include "PacketEncoder.h"
#include "PacketDecoder.h"
#include "RealUARTBus.h"

//#define SENDER  // Comment this out on the receiver board

const int TX_PIN = 17;
const int RX_PIN = 18;

HardwareSerial mySerial(2); // UART2
RealUARTBus uart(mySerial);
PacketEncoder encoder;
PacketDecoder decoder;

void setup() {
    Serial.begin(115200);
    mySerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
    Serial.println("Ready.");
}

void loop() {
#ifdef SENDER
    std::vector<uint8_t> payload = {0x01, 0x02, 0x03};
    auto packet = encoder.encode(0x01, payload);
    uart.sendBytes(packet);
    Serial.println("Packet sent.");
    delay(1000);
#else
    auto raw = uart.readBytes(7);
    // Serial.print("Raw bytes: ");
    // Serial.print(raw[0], HEX);
    // Serial.print(" | size: ");
    // Serial.println(raw.size());
    // delay(100);
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
#endif
}