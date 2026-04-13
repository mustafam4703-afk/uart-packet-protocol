#ifndef IUARTBUS_H
#define IUARTBUS_H

#include <cstdint>
#include <vector>

class IUARTBus {
public:
    virtual ~IUARTBus() = default;
    virtual void sendBytes(const std::vector<uint8_t>& bytes) = 0;
    virtual std::vector<uint8_t> readBytes(size_t count) = 0;
};

#endif // IUARTBUS_H