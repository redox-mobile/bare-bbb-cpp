import qbs

BareLibrary {
    name: "base"

    Depends { name: "starterware" }

    cpp.includePaths: [
        product.sourceDirectory
    ]

    files: [
        "hal/*.cpp", "hal/*.h",
        "hw/*.cpp", "hw/*.h",
        "os/*.cpp", "os/*.h",
    ]

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: [
            product.sourceDirectory
        ]
    }
}
