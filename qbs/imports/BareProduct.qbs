import qbs

Product {
    Depends { name: "cpp" }

    cpp.defines: [
        "beaglebone",
        "gcc",
        "am3358",
        "SUPPORT_UNALIGNED",
        "MMCSD",
        "UARTCONSOLE"
    ]

    cpp.staticLibraries: [
        "c",
        "gcc",
        "nosys",
        "stdc++"
    ]
}
