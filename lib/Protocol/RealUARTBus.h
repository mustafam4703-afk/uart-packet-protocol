#ifndef REAL_UART_BUS_H
#define REAL_UART_BUS_H

#include "IUARTBus.h"
#include <HardwareSerial.h>

class RealUARTBus : public IUARTBus {
public:
    RealUARTBus(HardwareSerial& serial) : _serial(serial) {}

    void sendBytes(const std::vector<uint8_t>& bytes) override {
        _serial.write(bytes.data(), bytes.size());
    }

std::vector<uint8_t> readBytes(size_t count) override {
    std::vector<uint8_t> buffer{};
    unsigned long startTime = millis();
    
    // Wait for start byte
    while (millis() - startTime < 1000) {
        if (_serial.available() && _serial.peek() == 0xAA) {
            break;
        } else if (_serial.available()) {
            _serial.read(); // discard non-start bytes
        }
    }
    
    // Read count bytes
    while (buffer.size() < count && millis() - startTime < 1000) {
        if (_serial.available()) {
            buffer.push_back(_serial.read());
        }
    }

    if(buffer.empty() || buffer.size() < count) {
        buffer.clear(); // Clear buffer if we didn't get enough data
        buffer.push_back(0x00); // Return a default value if no data is received
    }
    
    return buffer;
}

private:
    HardwareSerial& _serial;
};

#endif // REAL_UART_BUS_H