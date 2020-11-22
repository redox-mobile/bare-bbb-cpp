#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include <stdint.h>

// StarterWare includes
#include <beaglebone.h>
#include <gpio_v2.h>
#include <hw_control_AM335x.h>
#include <soc_AM335x.h>

namespace HAL {

enum GpioDirection {
    GPIO_INPUT = 0,
    GPIO_OUTPUT
};

enum GpioLevel {
    GPIO_LOW = 0,
    GPIO_HIGH
};

namespace Internal {

extern const uint32_t GPIO_BASE_ADDRS[];

}

void gpioInit();

inline void gpioSetDirection(uint8_t module, uint16_t number, GpioDirection direction) {
    GPIODirModeSet(Internal::GPIO_BASE_ADDRS[module], number,
                   (direction == GPIO_INPUT) ? GPIO_DIR_INPUT : GPIO_DIR_OUTPUT);
}

inline void gpioSetLevel(uint8_t module, uint16_t number, GpioLevel level) {
    GPIOPinWrite(Internal::GPIO_BASE_ADDRS[module], number,
                 (level == GPIO_LOW) ? GPIO_PIN_LOW : GPIO_PIN_HIGH);
}

} // namespace HAL

#endif // #ifndef HAL_GPIO_H
