#include "gpio.h"

// StarterWare includes
#include <beaglebone.h>
#include <gpio_v2.h>
#include <soc_AM335x.h>

namespace HW {

Gpio *Gpio::m_instance = 0;

namespace Internal {

/*
 * GpioPrivate
 */
class GpioPrivate {

public:
    uint32_t leds;
};

} // namespace Internal

/*
 * Gpio
 */
Gpio& Gpio::instance() {
    if (!m_instance) {
        m_instance = new Gpio();
        m_instance->ctor();
    }
    return *m_instance;
}

void Gpio::ctor() {
    d = new Internal::GpioPrivate();
    GPIO1ModuleClkConfig();
    GPIO1Pin23PinMuxSetup();
    GPIOModuleEnable(SOC_GPIO_1_REGS);
    GPIOModuleReset(SOC_GPIO_1_REGS);
    GPIODirModeSet(SOC_GPIO_1_REGS,
                   23,
                   GPIO_DIR_OUTPUT);
}

void Gpio::ledOff(uint32_t ledMask) {
    GPIOPinWrite(SOC_GPIO_1_REGS,
                 23,
                 GPIO_PIN_LOW);
}

void Gpio::ledInvert(uint32_t ledMask) {
    d->leds ^= ledMask;
    GPIOPinWrite(SOC_GPIO_1_REGS,
                 23,
                 (d->leds & GPIO_LED_0) ? GPIO_PIN_HIGH : GPIO_PIN_LOW);
}

} // namespace HW
