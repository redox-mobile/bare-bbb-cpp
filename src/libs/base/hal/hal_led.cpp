#include "hal_led.h"
#include "hal_gpio.h"

namespace HAL {

namespace Internal {

static const uint16_t LED0_GPIO = 21;
static const uint16_t LED1_GPIO = 22;
static const uint16_t LED2_GPIO = 23;
static const uint16_t LED3_GPIO = 24;

const uint16_t LED_GPIO_PINS[] = { LED0_GPIO, LED1_GPIO, LED2_GPIO, LED3_GPIO };

} // namespace Internal

void ledInit() {
    gpioInit();
    for (uint16_t i = 0; i < LED_COUNT; ++i) {
        gpioSetDirection(Internal::LED_GPIO_MODULE, Internal::LED_GPIO_PINS[i], GPIO_OUTPUT);
        gpioSetLevel(Internal::LED_GPIO_MODULE, Internal::LED_GPIO_PINS[i], GPIO_LOW);
    }
}

} // namespace HAL
