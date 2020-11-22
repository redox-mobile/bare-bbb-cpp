#ifndef HAL_TIMER_H
#define HAL_TIMER_H

#include <stdint.h>

// StarterWare includes
#include <beaglebone.h>
#include <dmtimer.h>
#include <interrupt.h>
#include <soc_AM335x.h>

namespace HAL {

namespace Internal {

const uint32_t  TIMER_BASE_ADDR  = SOC_DMTIMER_2_REGS;
const uint8_t   TIMER_INTERRUPT  = SYS_INT_TINT2;
const uint32_t  TIMER_CLOCK_MHZ  = 24;

}

inline void timerInit() {
    DMTimer2ModuleClkConfig();
    DMTimerPreScalerClkDisable(Internal::TIMER_BASE_ADDR);
}

inline void timerInterruptClear() {
    DMTimerIntStatusClear(Internal::TIMER_BASE_ADDR, DMTIMER_INT_OVF_IT_FLAG);
}

inline void timerInterruptCleanUp() {
    timerInterruptClear();
}

inline void timerInterruptEnable() {
    DMTimerIntEnable(Internal::TIMER_BASE_ADDR, DMTIMER_INT_OVF_EN_FLAG);
}

inline void timerInterruptEnter() {
    // Disable & clear
    DMTimerIntDisable(Internal::TIMER_BASE_ADDR, DMTIMER_INT_OVF_EN_FLAG);
    timerInterruptClear();
}

inline void timerInterruptExit() {
    // Reenable
    DMTimerIntEnable(Internal::TIMER_BASE_ADDR, DMTIMER_INT_OVF_EN_FLAG);
}

inline void timerSetIsr(void (*isr)()) {
    // Setup ISR
    IntRegister(Internal::TIMER_INTERRUPT, isr);
    IntPrioritySet(Internal::TIMER_INTERRUPT, 0, AINTC_HOSTINT_ROUTE_IRQ);
    IntSystemEnable(Internal::TIMER_INTERRUPT);
}

inline void timerSetPeriodUs(uint32_t periodUs) {
    DMTimerCounterSet(Internal::TIMER_BASE_ADDR, 0xFFFFFFFF - (Internal::TIMER_CLOCK_MHZ * periodUs));
    DMTimerReloadSet(Internal::TIMER_BASE_ADDR, 0xFFFFFFFF - (Internal::TIMER_CLOCK_MHZ * periodUs));
    DMTimerModeConfigure(Internal::TIMER_BASE_ADDR, DMTIMER_AUTORLD_NOCMP_ENABLE);
}

} // namespace HAL

#endif // #ifndef HAL_TIMER_H
