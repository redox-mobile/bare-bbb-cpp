#include "hal_gpio.h"

#include <hw_cm_per.h>
#include <hw_types.h>

namespace HAL {

namespace Internal {

static const uint8_t GPIO_MODULE_COUNT = 3;

const uint32_t GPIO_BASE_ADDRS[] = { SOC_GPIO_0_REGS, SOC_GPIO_1_REGS, SOC_GPIO_2_REGS };

void gpio2ModuleClkConfig(void)
{

    /* Writing to MODULEMODE field of CM_PER_GPIO1_CLKCTRL register. */
    HWREG(SOC_CM_PER_REGS + CM_PER_GPIO2_CLKCTRL) |=
            CM_PER_GPIO2_CLKCTRL_MODULEMODE_ENABLE;

    /* Waiting for MODULEMODE field to reflect the written value. */
    while (CM_PER_GPIO2_CLKCTRL_MODULEMODE_ENABLE !=
           (HWREG(SOC_CM_PER_REGS + CM_PER_GPIO2_CLKCTRL) &
            CM_PER_GPIO2_CLKCTRL_MODULEMODE)) {}
    /*
    ** Writing to OPTFCLKEN_GPIO_2_GDBCLK bit in CM_PER_GPIO2_CLKCTRL
    ** register.
    */
    HWREG(SOC_CM_PER_REGS + CM_PER_GPIO2_CLKCTRL) |=
            CM_PER_GPIO2_CLKCTRL_OPTFCLKEN_GPIO_2_GDBCLK;

    /*
    ** Waiting for OPTFCLKEN_GPIO_2_GDBCLK bit to reflect the desired
    ** value.
    */
    while (CM_PER_GPIO2_CLKCTRL_OPTFCLKEN_GPIO_2_GDBCLK !=
           (HWREG(SOC_CM_PER_REGS + CM_PER_GPIO2_CLKCTRL) &
            CM_PER_GPIO2_CLKCTRL_OPTFCLKEN_GPIO_2_GDBCLK)) {}

    /*
    ** Waiting for IDLEST field in CM_PER_GPIO2_CLKCTRL register to attain the
    ** desired value.
    */
    while ((CM_PER_GPIO2_CLKCTRL_IDLEST_FUNC <<
            CM_PER_GPIO2_CLKCTRL_IDLEST_SHIFT) !=
           (HWREG(SOC_CM_PER_REGS + CM_PER_GPIO2_CLKCTRL) &
            CM_PER_GPIO2_CLKCTRL_IDLEST)) {}

    /*
    ** Waiting for CLKACTIVITY_GPIO_2_GDBCLK bit in CM_PER_L4LS_CLKSTCTRL
    ** register to attain desired value.
    */
    while (CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_GPIO_2_GDBCLK !=
           (HWREG(SOC_CM_PER_REGS + CM_PER_L4LS_CLKSTCTRL) &
            CM_PER_L4LS_CLKSTCTRL_CLKACTIVITY_GPIO_2_GDBCLK)) {}
}

} // namespace Internal

void gpioInit() {
    static bool isInitialized = false;
    if (!isInitialized) {
        GPIO0ModuleClkConfig();
        GPIO1ModuleClkConfig();
        Internal::gpio2ModuleClkConfig();

        for (uint8_t i = 0; i < Internal::GPIO_MODULE_COUNT; ++i) {
            GPIOModuleEnable(Internal::GPIO_BASE_ADDRS[i]);
            GPIOModuleReset(Internal::GPIO_BASE_ADDRS[i]);
        }

        isInitialized = true;
    }
}

} // namespace HAL
