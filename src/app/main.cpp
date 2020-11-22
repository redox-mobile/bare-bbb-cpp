#include "app.h"

#include <hw/hw.h>

int main(void) {
    // Primary hardware initialization
    HW::init();

    // Application instance creation
    App &a = app();

    // Application initialization
    // Includes memory allocation for objects used
    a.init();

    // TODO: Add errors handling

    // Start application execution
    a.exec();

    return 0; // Unreachable under normal conditions
}
