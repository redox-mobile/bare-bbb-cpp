#include "app.h"

#include <hw/adc.h>
#include <hw/can.h>
#include <hw/led.h>
#include <hw/timer.h>

#include <os/os.h>

App::App() {
    m_led   = 0;
    m_timer = 0;
    m_adc   = 0;
    m_can   = 0;
}

// End-of-Conversion interrupt service routine
void App::adcEoc() {
    // Invert LED
    static bool ledOn = true;
    app().led()->toggle(LED_2, ledOn);
    ledOn = !ledOn;
}

void App::exec() {
    OS::start();
}

void App::init() {
    m_led = new HW::Led();

    m_led = new HW::Led();
    if (m_led)
        m_led->switchOn(LED_0);

    m_adc = new HW::Adc();
    if (m_adc) {
        m_adc->setFrequencyHz(1000000); // 1 MHz
        m_adc->setCallback(adcEoc);
    }

    m_can = new HW::Can();
    if (m_can) {
        m_can->setBitRate(1000000); // 1 Mbit/s (for PixHawk/ArduPilot)
        m_can->setFormat(HW::Can::ExtendedFormat);
        m_can->clear();
    }

    m_timer = new HW::Timer();
    if (m_timer) {
        m_timer->setPeriodUs(100000); // 100 ms => 10 Hz
        m_timer->setCallback(timerTick);
    }
}

App& App::instance() {
    static App *appInstance = 0;
    if (!appInstance) {
        // Application instance creation
        appInstance = new App();

        if (!appInstance)
            OS::abort();
        // TODO: Add allocation errors handling
    }
    return *appInstance;
}

void App::timerTick() {
    // Invert LED
    static bool ledOn = true;
    app().led()->toggle(LED_1, ledOn);
    ledOn = !ledOn;
}
