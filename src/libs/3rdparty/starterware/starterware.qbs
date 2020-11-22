import qbs

BareLibrary {
    name: "starterware"

    cpp.includePaths: [
        product.sourceDirectory,
        product.sourceDirectory + "/include",
        product.sourceDirectory + "/include/hw",
        product.sourceDirectory + "/include/armv7a",
        product.sourceDirectory + "/include/armv7a/am335x"
    ]

    Group {
        name: "Headers"
        prefix: "include/"
        files: [
            "*.h",
            "hw/*.h",
            "armv7a/*.h",
            "armv7a/am335x/*.h"
        ]
    }

    Group {
        name: "Drivers"
        prefix: "drivers/"
        files: [
            "gpio_v2.c",
            "dcan.c",
            "dmtimer.c",
            "tsc_adc.c",
            "watchdog.c"
        ]
    }

    Group {
        name: "Examples"
        prefix: "examples/evmAM335x/dcanTxRx/"
        files: [
            "dcan_frame.c", "dcan_frame.h"
        ]
    }

    Group {
        name: "Platform"
        prefix: "platform/"
        files: [
            "beaglebone/gpio.c",
            "beaglebone/dmtimer.c",
            "beaglebone/watchdog.c",
            "evmAM335x/dcan.c",
            "evmskAM335x/tscAdc.c"
        ]
    }

    Group {
        name: "System Config (GCC)"

        prefix: "system_config/armv7a/"
        files: [
            "cache.c",
            "gcc/cp15.S",
            "gcc/cpu.c",
            "am335x/interrupt.c",
            "am335x/startup.c",
            "am335x/gcc/exceptionhandler.S"
        ]
    }

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [
            product.sourceDirectory,
            product.sourceDirectory + "/include",
            product.sourceDirectory + "/include/hw",
            product.sourceDirectory + "/include/armv7a",
            product.sourceDirectory + "/include/armv7a/am335x",
            product.sourceDirectory + "/examples/evmAM335x/dcanTxRx"
        ]
    }
}
