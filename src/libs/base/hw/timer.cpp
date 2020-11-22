#include "timer.h"

#include <hal/hal_irq.h>
#include <hal/hal_timer.h>

namespace HW {

/*
 * TimerPrivate
 */
namespace Internal {

class TimerPrivate {
public:
    static void isr();

public:
    static void (*callback)();
};

void (*TimerPrivate::callback)() = 0;

void TimerPrivate::isr() {
    HAL::timerInterruptEnter();

    if (callback)
        callback();

    HAL::timerInterruptExit();
}

} // namespace Internal

/*
 * Timer
 */
Timer::Timer() {
    HAL::timerInit();
    d = new Internal::TimerPrivate();
}

void Timer::setPeriodUs(uint32_t periodUs) {
    HAL::timerSetPeriodUs(periodUs);
}

void Timer::setCallback(void (*callback)()) {
    d->callback = callback;

    HAL::timerSetIsr(Internal::TimerPrivate::isr);
    HAL::timerInterruptCleanUp();
    HAL::timerInterruptEnable();
    HAL::irqEnable();
}

} // namespace HW
