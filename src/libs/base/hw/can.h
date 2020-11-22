#ifndef HW_CAN_H
#define HW_CAN_H

#include <stddef.h>
#include <stdint.h>

namespace HW {

namespace Internal { class CanPrivate; }

class Can {
public:
    enum Format {
        AutoFormat = 0,   // Detects how many bits are used in CAN-ID
        StandardFormat,   // 11-bit CAN-ID
        ExtendedFormat    // 29-bit CAN-ID
    };

    Can();

    void setCallback(void (*callback)());
    void setBitRate(uint32_t bitRate);

    void setId(uint32_t canId);
    void setFormat(Format format);
    void setFilterId(uint32_t canId);

    void clear();
    bool write(void *buffer, size_t length);
    bool read(void *buffer, size_t length);

private:
    Internal::CanPrivate *d;
};

} // namespace HW

#endif // #ifndef HW_CAN_H
