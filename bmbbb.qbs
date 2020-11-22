import qbs

Project {
    // Version info
    property int versionMajor: 0
    property int versionMinor: 1
    property int versionRelease: 0

    property path sourcePath: path + "/src"
    property path libsPath: sourcePath + "/libs"
    property path thirdPartyPath: libsPath + "/3rdparty"

    qbsSearchPaths: [
        "qbs"
    ]

    references: [
        "src/src.qbs"
    ]
}

