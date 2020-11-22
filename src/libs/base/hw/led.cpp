#include "led.h"

#include <hal/hal_led.h>

namespace HW {

Led::Led() {
    HAL::ledInit();
}

void Led::switchOff(uint8_t number) {
    HAL::ledSwitchOff(number);
}

void Led::switchOn(uint8_t number) {
    HAL::ledSwitchOn(number);
}

} // namespace HW
