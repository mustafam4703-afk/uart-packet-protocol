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
        std::vector<uint8_t> buffer;
        while (buffer.size() < count && _serial.available()) {
            buffer.push_back(_serial.read());
        }
        return buffer;
    }

private:
    HardwareSerial& _serial;
};

#endif // REAL_UART_BUS_H