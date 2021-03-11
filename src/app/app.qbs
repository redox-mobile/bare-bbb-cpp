import qbs

BareApp {
    name: "bare-bbb-cpp-" + project.versionMajor + "." + project.versionMinor + "." + project.versionRelease

    Depends { name: "base" }
    Depends { name: "starterware" }

    cpp.heapSize: 0x100000  // 1 Mbyte
    cpp.stackSize: 0x100000 // 1 Mbyte

    files: [
        "app.cpp", "app.h",
        "main.cpp"
    ]

    Group {
        name: "AM335X"
        files: [
            "am335x.lds",
            "init.S"
        ]
    }

    Group {
        fileTagsFilter: ["binary", "uenv_txt"]
        qbs.install: true
        qbs.installDir: ""
        qbs.installPrefix: ""
    }
}
