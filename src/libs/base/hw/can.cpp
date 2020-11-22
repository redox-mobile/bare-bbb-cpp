#include "can.h"

#include <hal/hal_can.h>
#include <hal/hal_irq.h>

namespace HW {

namespace Internal {

/*
 * CanPrivate
 */
class CanPrivate {
public:
    static void isr();

public:
    static void (*callback)(void);
    uint32_t canId;
    Can::Format format;
};

void (*CanPrivate::callback)(void) = 0;

void CanPrivate::isr() {
    HAL::canInterruptEnter();

    if (callback)
        callback();

    HAL::canInterruptExit();
}

} // namespace Internal

/*
 * Can
 */
Can::Can() {
    HAL::canInit();
    d = new Internal::CanPrivate();
    d->canId = 0;
    d->format = AutoFormat;
}

void Can::clear() {
    HAL::canCleanUp();
}

bool Can::read(void *buffer, size_t length) {
    // TODO: implement
    return true;
}

void Can::setBitRate(uint32_t bitRate) {
    HAL::canSetBitRate(bitRate);
}

void Can::setCallback(void (*callback)()) {
    d->callback = callback;

    HAL::canSetIsr(Internal::CanPrivate::isr);
    //HAL::canSetErrorIsr(Internal::CanPrivate::errorIsr);
    HAL::canInterruptCleanUp();
    HAL::canInterruptEnable();
    HAL::irqEnable();
}

void Can::setFilterId(uint32_t canId) {
    bool extendedFormat = (d->format == ExtendedFormat);
    if (d->format == AutoFormat) {
        extendedFormat = (canId & 0xFFFFF800) > 0;
    }
    HAL::canConfigRx(canId, extendedFormat);
}

void Can::setFormat(Format format) {
    d->format = format;
}

void Can::setId(uint32_t canId) {
    d->canId = canId;
}

bool Can::write(void *buffer, size_t length) {
    bool extendedFormat = (d->format == ExtendedFormat);
    if (d->format == AutoFormat) {
        extendedFormat = (d->canId & 0xFFFFF800) > 0;
    }
    HAL::canWrite(d->canId, buffer, length, extendedFormat);
    return true;
}


} // namespace HW
