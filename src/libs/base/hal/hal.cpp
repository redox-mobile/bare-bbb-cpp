#include "hal.h"

// StarterWare includes
#include <beaglebone.h>
#include <cache.h>
#include <cp15.h>
#include <interrupt.h>
#include <soc_AM335x.h>
#include <watchdog.h>

namespace HAL {

void init() {
    // Invalidate all cache
    CacheInstInvalidateAll();
    CacheDataCleanInvalidateAll();

    // Disable MMU
    CP15MMUDisable();

    // Reset interrupt controller
    IntAINTCInit();

    // Disable watchdog
    WatchdogTimer1ModuleClkConfig();
    WatchdogTimerReset(SOC_WDT_1_REGS);
    WatchdogTimerDisable(SOC_WDT_1_REGS);
}

} // namespace HAL
