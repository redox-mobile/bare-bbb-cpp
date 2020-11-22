#pragma once

#include <cstdint>

const uint8_t LED_0 = 0;
const uint8_t LED_1 = 1;
const uint8_t LED_2 = 2;

// Forward class declarations
namespace HW {

class Adc;
class Can;
class Led;
class Timer;

}

// Main application singleton
class App {
public:
    static App& instance();
    void init();
    void exec();

    HW::Led* led() const { return m_led; }
    HW::Adc* adc() const { return m_adc; }
    HW::Can* can() const { return m_can; }

private:
    App();
    App(App const &);
    void operator=(App const &);

    static void timerTick();
    static void adcEoc();

private:
    HW::Led     *m_led;
    HW::Timer   *m_timer;
    HW::Adc     *m_adc;
    HW::Can     *m_can;

};

static inline App& app() {
    return App::instance();
}
