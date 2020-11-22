import qbs
import qbs.File
import qbs.ModUtils
import qbs.Process
import qbs.TextFile

Module {
    property pathList includePaths
    property pathList libraryPaths

    property stringList defines
    property stringList staticLibraries

    property stringList symbols

    property int stackSize: 0x1C000
    property int heapSize: 0x14000

    property string optimization: qbs.optimization
    property bool debugInformation: qbs.debugInformation

    property string compilerName: "arm-none-eabi-gcc"
    property string linkerName: "arm-none-eabi-ld"

    // Derived from qbs.conf (declared in order to avoid warnings)
    property var compilerPathByLanguage
    property string minimumWindowsVersion
    property string platformCFlags
    property string platformCxxFlags
    property string toolchainInstallPath
    property string toolchainPrefix

    Rule {
        id: uenvGenerator
        inputs: ["binary"]

        Artifact {
            filePath: product.destinationDirectory + "/uEnv.txt"
            fileTags: ["uenv_txt"]
        }

        prepare: {
            var cmd = new JavaScriptCommand();
            cmd.description = "creating " + output.fileName;
            cmd.highlight = "filegen";
            cmd.sourceCode = function() {
                var uEnvTxt = new TextFile(output.filePath, TextFile.WriteOnly);
                uEnvTxt.writeLine("uenvcmd=fatload mmc 0 0x80000000 " + input.fileName + " && go 0x80000000");
                uEnvTxt.close();
            };
            return cmd;
        }
    }

    Rule {
        id: binaryGenerator
        inputs: ["executable"]

        Artifact {
            filePath: product.destinationDirectory + "/" + product.targetName + ".bin"
            fileTags: ["binary"]
        }

        prepare: {
            var args = [];

            args.push("-O", "binary");
            args.push(input.filePath);
            args.push(output.filePath);

            var cmd = new Command("arm-none-eabi-objcopy", args);
            cmd.description = "creating " + output.fileName;
            cmd.highlight = "filegen";
            cmd.responseFileUsagePrefix = "@";
            cmd.workingDirectory = product.destinationDirectory;
            return cmd;
        }
    }

    Rule {
        id: libraryLinker
        multiplex: true
        inputs: ["obj"]

        Artifact {
            filePath: product.destinationDirectory + "/" + "lib" + product.targetName + ".a"
            fileTags: ["library"]
        }

        prepare: {
            var args = [];

            args.push("-c", "-r");

            args.push(output.filePath);

            for (var i in inputs.obj)
                args.push(inputs.obj[i].filePath);

            var cmd = new Command("arm-none-eabi-ar", args);
            cmd.description = "linking " + output.fileName;
            cmd.highlight = "linker";
            cmd.responseFileUsagePrefix = "@";
            cmd.workingDirectory = product.destinationDirectory;
            return cmd;
        }
    }

    Rule {
        id: executableLinker
        multiplex: true
        inputs: ["obj", "lds"]
        inputsFromDependencies: ["library"]

        Artifact {
            filePath: product.destinationDirectory + "/" + product.targetName + ".out"
            fileTags: ["executable"]
        }

        prepare: {
            var args = [];

            args.push("-mfloat-abi=softfp"); // Specify if floating point hardware should be used

            if (ModUtils.moduleProperty(product, "debugInformation"))
                args.push("-g", "-gdwarf-3", "-gstrict-dwarf");
            var opt = ModUtils.moduleProperty(product, "optimization")
            if (opt === "fast")
                args.push("-g", "-O2");
            if (opt === "small")
                args.push("-Os");

            args.push("-Wl,-Map," + product.targetName + ".map"); // Write a map file

            var symbols = ModUtils.moduleProperty(product, "symbols");
            for (i in symbols)
                args.push("-Wl,--defsym," + symbols[i]);

            args.push("-ffunction-sections"); // Place each function into its own section
            args.push("-fdata-sections"); // Place data items into their own section

            var libraryPaths = ModUtils.moduleProperty(product, "libraryPaths");
            for (i in libraryPaths)
                args.push("-L" + libraryPaths[i]);

            args.push("-o", output.filePath); // Output file

            for (var i in inputs.obj)
                args.push(inputs.obj[i].filePath);

            for (var i in inputs.lds)
                args.push("-Wl,-T" + inputs.lds[i].filePath); // Linker commad files

            args.push("-Wl,--start-group"); // Start a group
            var staticLibraries = ModUtils.moduleProperty(product, "staticLibraries");
            for (var i in inputs.library)
                staticLibraries.push(inputs.library[i].filePath);
            for (i in staticLibraries) {
                if (File.exists(staticLibraries[i]))
                    args.push(staticLibraries[i]);
                else
                    args.push('-l' + staticLibraries[i]);
            }
            args.push("-Wl,--end-group"); // End a group

            var cmd = new Command("arm-none-eabi-gcc", args);
            cmd.description = "linking " + output.fileName;
            cmd.highlight = "linker";
            cmd.responseFileUsagePrefix = "@";
            cmd.workingDirectory = product.destinationDirectory;
            return cmd;
        }
    }

    Rule {
        id: compiler
        inputs: ["cpp", "c", "asm", "asm_cpp"]
        auxiliaryInputs: ["hpp"]

        Artifact {
            fileTags: ["obj"]
            filePath: ".obj/" + input.baseDir + "/" + input.baseName + ".o"
        }

        prepare: {
            var args = [];

            //args.push("-mcpu=cortex-a8"); // Target CPU (deprecated)
            args.push("-mtune=cortex-a8"); // Tune code for given processor
            args.push("-march=armv7-a"); // Specify the name of the target architechture
            args.push("-marm"); // Compile for ARM (not Thumb)

            if (ModUtils.moduleProperty(product, "debugInformation"))
                args.push("-g", "-gdwarf-3", "-gstrict-dwarf");
            var opt = ModUtils.moduleProperty(product, "optimization")
            if (opt === "fast")
                args.push("-g", "-O2");
            if (opt === "small")
                args.push("-Os");

            args.push("-ffunction-sections"); // Place each function into its own section
            args.push("-fdata-sections"); // Place data items into their own section
            args.push("-mlong-calls");
            args.push("-funsigned-char");

            args.push("-mfpu=neon"); // Specify the name of the target floating point hardware/format
            args.push("-mfloat-abi=softfp"); // Specify if floating point hardware should be used

            args.push("-Wall"); // Enable most warning messages

            var defines = ModUtils.moduleProperty(product, "defines");
                for (i in defines)
                    args.push("-D" + defines[i]);

            var includePaths = ModUtils.moduleProperty(product, "includePaths");
            for (i in includePaths)
                args.push("-I" + includePaths[i]);

            args.push("-c"); // Compile and assemble, but do not link
            args.push(input.filePath);
            args.push("-o");
            args.push(output.filePath);

            var cmd = new Command("arm-none-eabi-gcc", args);
            cmd.description = "compiling " + input.fileName;
            cmd.highlight = "compiler";
            cmd.responseFileUsagePrefix = "@";
            return cmd;
        }
    }

    // C sources
    FileTagger {
        patterns: ["*.c"]
        fileTags: ["c"]
    }

    // C++ sources
    FileTagger {
        patterns: ["*.C", "*.cpp", "*.cxx", "*.c++", "*.cc"]
        fileTags: ["cpp"]
    }

    // Headers
    FileTagger {
        patterns: ["*.h", "*.H", "*.hpp", "*.hxx", "*.h++"]
        fileTags: ["hpp"]
    }

    // Linker scripts
    FileTagger {
        patterns: ["*.ld", "*.lds"]
        fileTags: ["lds"]
    }

    // Pure assembler sources
    FileTagger {
        patterns: "*.s"
        fileTags: ["asm"]
    }

    // Mixed assembler and C/C++ sources
    FileTagger {
        patterns: "*.S"
        fileTags: ["asm_cpp"]
    }
}
