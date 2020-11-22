#include "adc.h"

#include <hal/hal_adc.h>
#include <hal/hal_irq.h>

namespace HW {

namespace Internal {

/*
 * AdcPrivate
 */
class AdcPrivate {
public:
    static void isr();

public:
    static void (*callback)(void);
    static uint16_t currentSample;
};

void (*AdcPrivate::callback)(void) = 0;
uint16_t AdcPrivate::currentSample = 0;

void AdcPrivate::isr() {
    HAL::adcInterruptEnter();

    if (HAL::adcEndOfConversion()) {
    	currentSample = HAL::adcRead();
        if (callback)
            callback();
    }

    HAL::adcInterruptExit();
}

} // namespace Internal

/*
 * Adc
 */
Adc::Adc() {
    HAL::adcInit();

    HAL::adcSetFrequencyHz(1000);
    HAL::adcSetChannel(0);

    d = new Internal::AdcPrivate();
}

void Adc::clear() {
	while (HAL::adcFifoWordCount() > 0)
		HAL::adcRead();
}

uint16_t Adc::sample() {
    return d->currentSample;
}

void Adc::setCallback(void (*callback)()) {
    d->callback = callback;

    HAL::adcSetIsr(Internal::AdcPrivate::isr);
    HAL::adcInterruptCleanUp();
    HAL::adcInterruptEnable();
    HAL::irqEnable();
}

void Adc::setChannel(uint16_t channel) {
    HAL::adcSetChannel(channel);
}

void Adc::setFrequencyHz(uint32_t frequencyHz) {
    HAL::adcSetFrequencyHz(frequencyHz);
}

void Adc::startOfConversion() {
    HAL::adcStartOfConversion();
}


} // namespace HW
