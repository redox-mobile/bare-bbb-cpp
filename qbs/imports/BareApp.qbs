import qbs

BareProduct {
    type: ["executable", "binary", "uenv_txt"]

    // Compiler defines
    cpp.defines: [
        "STACKSIZE=" + cpp.stackSize,
        "HEAPSIZE=" + cpp.heapSize
    ]

    // Linker symbols
    cpp.symbols: [
        "STACKSIZE=" + cpp.stackSize,
        "HEAPSIZE=" + cpp.heapSize
    ]
}
