#ifndef HW_LED_H
#define HW_LED_H

#include <stdint.h>

namespace HW {

namespace Internal { class LedPrivate; }

class Led {
public:
    Led();

    void toggle(uint8_t number, bool lightUp) { lightUp ? switchOn(number) : switchOff(number); }
    void switchOff(uint8_t number);
    void switchOn(uint8_t number);
};

extern Led& led();

} // namespace HW

#endif // #ifndef HW_GPIO_H
