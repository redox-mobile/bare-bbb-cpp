#ifndef HAL_LED_H
#define HAL_LED_H

#include "hal_gpio.h"

namespace HAL {

const uint16_t LED_COUNT = 4;

namespace Internal {

const uint16_t LED_GPIO_MODULE = 1;

extern const uint16_t LED_GPIO_PINS[];

}

inline void ledSwitchOff(uint16_t number) {
    if (number < LED_COUNT)
        gpioSetLevel(Internal::LED_GPIO_MODULE, Internal::LED_GPIO_PINS[number], GPIO_LOW);
}

inline void ledSwitchOn(uint16_t number) {
    if (number < LED_COUNT)
        gpioSetLevel(Internal::LED_GPIO_MODULE, Internal::LED_GPIO_PINS[number], GPIO_HIGH);
}

extern void ledInit();

} // namespace HAL

#endif // #ifndef HAL_LED_H
