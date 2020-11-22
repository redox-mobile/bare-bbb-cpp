#ifndef HAL_IRQ_H
#define HAL_IRQ_H

#include <stdint.h>

// StarterWare includes
#include <interrupt.h>

namespace HAL {

inline void irqEnable() {
    IntMasterIRQEnable();
}

} // namespace HAL

#endif // #ifndef HAL_IRQ_H
