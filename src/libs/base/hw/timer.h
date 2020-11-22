#ifndef HW_TIMER_H
#define HW_TIMER_H

#include <stdint.h>

namespace HW {

namespace Internal { class TimerPrivate; }

class Timer {
public:
    Timer();

    void setPeriodUs(uint32_t periodUs);
    void setCallback(void (*callback)());

private:
    Internal::TimerPrivate *d;
};

} // namespace HW

#endif // #ifndef HW_TIMER_H
