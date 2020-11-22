#ifndef HW_GPIO_H
#define HW_GPIO_H

#include <stdint.h>

namespace HW {

const uint32_t GPIO_LED_ALL = 0xFFFFFFFF;
const uint32_t GPIO_LED_0   = 0x1;

namespace Internal { class GpioPrivate; }

class Gpio {
public:
    static Gpio& instance();

    void ledOff(uint32_t ledMask = GPIO_LED_ALL);
    void ledInvert(uint32_t ledMask = GPIO_LED_ALL);

private:
    Gpio() {}
    Gpio(Gpio const &);
    void operator=(Gpio const &);

    void ctor();

private:
    static Gpio *m_instance;
    Internal::GpioPrivate *d;
};

} // namespace HW

#endif // #ifndef HW_GPIO_H
