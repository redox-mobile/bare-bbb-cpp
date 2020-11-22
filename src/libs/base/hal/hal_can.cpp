#include "hal_can.h"

// StarterWare includes
#include <evmAM335x.h>
#include <hw_control_AM335x.h>
#include <hw_types.h>
#include <pin_mux.h>

// Workaround in order to link with "evmAM335x/dcan.c"
unsigned int EVMProfileGet(void) {
    return 1;
}

namespace HAL {

namespace Internal {

static void dcan1MsgRamInit() {
	HWREG(SOC_CONTROL_REGS + CONTROL_DCAN_RAMINIT) &= ~CONTROL_DCAN_RAMINIT_DCAN1_RAMINIT_START;
    HWREG(SOC_CONTROL_REGS + CONTROL_DCAN_RAMINIT) |= CONTROL_DCAN_RAMINIT_DCAN1_RAMINIT_START;

    while (!(HWREG(SOC_CONTROL_REGS + CONTROL_DCAN_RAMINIT) & CONTROL_DCAN_RAMINIT_DCAN1_RAMINIT_DONE)) {}
}

static void dcan1PinMuxSetUp() {
    // Pin Mux for DCAN1 Tx Pin (D16)
    // Fast Slew Rate - Receiver Disabled - Pulldown - PU/PD feature Enabled - Mode 2
    HWREG(SOC_CONTROL_REGS + GPIO_0_14) = PAD_FS_RXD_PD_PUPDE(2);

    // Pin Mux for DCAN1 Rx Pin (D15)
    // Fast Slew Rate - Receiver Enabled - Pullup - PU/PD feature Enabled - Mode 2
    HWREG(SOC_CONTROL_REGS + GPIO_0_15) = PAD_FS_RXE_PU_PUPDE(2);
}

static void dcan1StartTransfer()
{
    // Start the CAN transfer
//    DCANNormalModeSet(Internal::CAN_BASE_ADDR);

    // Enable the error interrupts
    //DCANIntEnable(Internal::CAN_BASE_ADDR, DCAN_ERROR_INT);

    // Enable the interrupt line 0 of DCAN module
    //DCANIntLineEnable(Internal::CAN_BASE_ADDR, DCAN_INT_LINE0);

    //DCANIntLineEnable(Internal::CAN_BASE_ADDR, DCAN_INT_LINE1);
}

} // namespace Internal

void canConfigRx(uint32_t canId, bool extendedFormat) {
    static can_frame frame;

    frame.id = canId;
    frame.flag = (CAN_DATA_FRAME | CAN_MSG_DIR_RX);
    if (extendedFormat) {
        frame.flag |= CAN_EXT_FRAME;
    }

    // Configure a transmit message object
    CANMsgObjectConfig(Internal::CAN_BASE_ADDR, &frame);
}

void canInit() {
    // Enable the DCAN1 module clock
    DCANModuleClkConfig();

    // Perform the pinmux for DCAN1
    Internal::dcan1PinMuxSetUp();

    // Initialize the DCAN1 message RAM
    Internal::dcan1MsgRamInit();

    // Reset the DCAN module
    DCANReset(Internal::CAN_BASE_ADDR);
}

void canWrite(uint32_t canId, void *buffer, uint8_t length, bool extendedFormat) {
    can_frame frame;

    frame.id = canId;
    frame.data = static_cast<unsigned int *>(buffer);
    frame.dlc = (length > 8) ? 8 : length;

    frame.flag = (CAN_DATA_FRAME | CAN_MSG_DIR_TX);
    if (extendedFormat) {
        frame.flag |= CAN_EXT_FRAME;
    }

    // Configure a transmit message object
    CANMsgObjectConfig(Internal::CAN_BASE_ADDR, &frame);

    // Start the CAN transfer
    Internal::dcan1StartTransfer();
}

} // namespace HAL
