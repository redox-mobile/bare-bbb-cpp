#ifndef HW_ADC_H
#define HW_ADC_H

#include <stdint.h>

namespace HW {

namespace Internal { class AdcPrivate; }

class Adc {
public:
    Adc();

    void setFrequencyHz(uint32_t frequencyHz);
    void setCallback(void (*callback)());

    void setChannel(uint16_t channel);
    void startOfConversion();
    uint16_t sample();

    void clear();

private:
    Internal::AdcPrivate *d;
};

} // namespace HW

#endif // #ifndef HW_ADC_H
