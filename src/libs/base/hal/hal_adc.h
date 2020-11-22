#ifndef HAL_ADC_H
#define HAL_ADC_H

#include <stdint.h>

// StarterWare includes
#include <beaglebone.h>
#include <interrupt.h>
#include <soc_AM335x.h>
#include <am335x_adc.h>

// Workaround
extern "C" {
#include <tsc_adc.h>
}

namespace HAL {

namespace Internal {

const uint32_t  ADC_BASE_ADDR       = SOC_ADC_TSC_0_REGS;
const uint8_t   ADC_INTERRUPT       = SYS_INT_ADC_TSC_GENINT;

const uint32_t  ADC_MODULE_CLOCK_HZ = 24000000;

inline void adcStepConfigure(uint32_t positiveIn) {
    // Configure ADC to Single ended operation mode
    TSCADCTSStepOperationModeControl(ADC_BASE_ADDR,
                                  TSCADC_SINGLE_ENDED_OPER_MODE, 0);

    // Configure step to select Channel, reference voltages
    TSCADCTSStepConfig(ADC_BASE_ADDR, 0, TSCADC_NEGATIVE_REF_VSSA,
                    positiveIn, TSCADC_NEGATIVE_INP_ADCREFM, TSCADC_POSITIVE_REF_VDDA);

    // Select FIFO
    TSCADCTSStepFIFOSelConfig(ADC_BASE_ADDR, 0, TSCADC_FIFO_0);

    // Configure ADC to one shot mode
    TSCADCTSStepModeConfig(ADC_BASE_ADDR, 0,  TSCADC_ONE_SHOT_SOFTWARE_ENABLED);
}

} // namespace Internal

inline bool adcEndOfConversion() {
    volatile uint32_t status = TSCADCIntStatus(Internal::ADC_BASE_ADDR);
    return (status & TSCADC_END_OF_SEQUENCE_INT) != 0;
}

inline uint32_t adcFifoWordCount() {
    return TSCADCFIFOWordCountRead(Internal::ADC_BASE_ADDR, TSCADC_FIFO_0);
}

inline void adcInit() {
    TSCADCModuleClkConfig();
    TSCADCPinMuxSetUp();

    // Disable Transistor bias
    TSCADCTSTransistorConfig(Internal::ADC_BASE_ADDR, TSCADC_TRANSISTOR_DISABLE);

    // Disable the channel ID store along with the captured ADC data
    TSCADCStepIDTagConfig(Internal::ADC_BASE_ADDR, 0);

    // Disable Write Protection of Step Configuration regs
    TSCADCStepConfigProtectionDisable(Internal::ADC_BASE_ADDR);

    // Analog inputs in general purpose mode (8 channels available)
    TSCADCTSModeConfig(Internal::ADC_BASE_ADDR, TSCADC_GENERAL_PURPOSE_MODE);

    // Delay for 100 clocks before SOC
    TSCADCTSStepOpenDelayConfig(Internal::ADC_BASE_ADDR, 0, 100);

    // Filter by averaging
    TSCADCTSStepAverageConfig(Internal::ADC_BASE_ADDR, 0, TSCADC_SIXTEEN_SAMPLES_AVG);

    // Enable the TSC_ADC_SS module
    TSCADCModuleStateSet(Internal::ADC_BASE_ADDR, TSCADC_MODULE_ENABLE);
}

inline void adcInterruptCleanUp() {
    TSCADCIntStatusClear(Internal::ADC_BASE_ADDR, 0x7FF);
    TSCADCIntStatusClear(Internal::ADC_BASE_ADDR, 0x7FF);
    TSCADCIntStatusClear(Internal::ADC_BASE_ADDR, 0x7FF);
}

inline void adcInterruptClear() {
    volatile uint32_t status = TSCADCIntStatus(Internal::ADC_BASE_ADDR);
    TSCADCIntStatusClear(Internal::ADC_BASE_ADDR, status);
}

inline void adcInterruptEnable() {
    TSCADCEventInterruptEnable(Internal::ADC_BASE_ADDR, TSCADC_END_OF_SEQUENCE_INT);
}

inline void adcInterruptEnter() {
    // Do nothing
}

inline void adcInterruptExit() {
    adcInterruptClear();
}

inline uint32_t adcRead() {
    return TSCADCFIFOADCDataRead(Internal::ADC_BASE_ADDR, TSCADC_FIFO_0);
}

inline void adcSetChannel(uint16_t channel) {
    // Configure step 1 for channel (AN0..AN7)
    if (channel <= TSCADC_POSITIVE_INP_CHANNEL8)
        Internal::adcStepConfigure(TSCADC_POSITIVE_INP_CHANNEL1 + channel);
}

inline void adcSetFrequencyHz(uint32_t frequencyHz) {
    TSCADCConfigureAFEClock(Internal::ADC_BASE_ADDR, Internal::ADC_MODULE_CLOCK_HZ, frequencyHz);
}

inline void adcSetIsr(void (*isr)()) {
    // Setup ISR
    IntRegister(Internal::ADC_INTERRUPT, isr);
    IntPrioritySet(Internal::ADC_INTERRUPT, 1, AINTC_HOSTINT_ROUTE_IRQ);
    IntSystemEnable(Internal::ADC_INTERRUPT);
}

inline void adcStartOfConversion() {
    // Enable step 1
    TSCADCConfigureStepEnable(Internal::ADC_BASE_ADDR, 1, 1);
}

} // namespace HAL

#endif // #ifndef HAL_ADC_H
