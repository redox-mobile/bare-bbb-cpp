#ifndef HAL_CAN_H
#define HAL_CAN_H

#include <stdint.h>

// StarterWare includes
#include <dcan.h>
#include <dcan_frame.h>
#include <interrupt.h>
#include <soc_AM335x.h>

namespace HAL {

namespace Internal {

const uint32_t  CAN_BASE_ADDR       = SOC_DCAN_1_REGS;
const uint32_t  CAN_INTERRUPT       = SYS_INT_DCAN1_INT0;
const uint32_t  CAN_PRIORITY        = 2;

const uint32_t  CAN_IN_CLK         = 24000000;

}

inline void canCleanUp() {
    uint32_t index = CAN_NUM_OF_MSG_OBJS;

    while (index--) {
        // Invalidate all message objects in the message RAM
        CANInValidateMsgObject(Internal::CAN_BASE_ADDR, index, DCAN_IF2_REG);
    }
}
/*
inline void canInterruptDisable() {
    const uint32_t msgNum = DCANIntRegStatusGet(SOC_DCAN_1_REGS, DCAN_INT_LINE0_STAT);
    CANRxIntDisable(Internal::CAN_BASE_ADDR, msgNum, DCAN_IF2_REG);
}
*/

inline void canInterruptCleanUp() {
}

inline void canInterruptEnable() {
    // Enable the interrupt line 0 of DCAN module
    DCANIntLineEnable(Internal::CAN_BASE_ADDR, DCAN_INT_LINE0);
}

inline void canInterruptEnter() {
}

inline void canInterruptExit() {
}

inline void canSetBitRate(uint32_t bitRate) {
    // Enable the write access to the DCAN configuration registers
    DCANConfigRegWriteAccessControl(Internal::CAN_BASE_ADDR, DCAN_CONF_REG_WR_ACCESS_ENABLE);

    // Enter the Initialization mode of CAN controller
    DCANInitModeSet(Internal::CAN_BASE_ADDR);

    // Configure the bit timing values for CAN communication
    CANSetBitTiming(Internal::CAN_BASE_ADDR, Internal::CAN_IN_CLK,
                    bitRate);

    // Disable the write access to the DCAN configuration registers
    DCANConfigRegWriteAccessControl(Internal::CAN_BASE_ADDR, DCAN_CONF_REG_WR_ACCESS_DISABLE);

    // Exit the Initialization mode of CAN controller
    DCANNormalModeSet(Internal::CAN_BASE_ADDR);
}

inline void canSetIsr(void (*isr)()) {
    // Setup ISR
    IntRegister(Internal::CAN_INTERRUPT, isr);
    IntPrioritySet(Internal::CAN_INTERRUPT, Internal::CAN_PRIORITY,
                   AINTC_HOSTINT_ROUTE_IRQ);
    IntSystemEnable(Internal::CAN_INTERRUPT);
}

extern void canConfigRx(uint32_t canId, bool extendedFormat = false);
extern void canInit();
extern void canWrite(uint32_t canId, void *buffer, uint8_t length, bool extendedFormat = false);

} // namespace HAL

#endif // #ifndef HAL_CAN_H
