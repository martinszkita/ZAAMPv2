# ZAAMPv2

## Overview
This repository contains a C++17-based command interpreter and a collection of runtime-loadable plugins that animate mobile objects for the ZAAMP project. The project now builds with CMake to ensure consistent compiler settings across the interpreter binary and all plugins.

## Building with CMake
1. Configure the project:
   ```bash
   cmake -S . -B build
   ```
2. Build the interpreter and plugins:
   ```bash
   cmake --build build
   ```

The top-level `CMakeLists.txt` pins C++17, disables compiler extensions, and adds the `zalazek` subproject. The interpreter (`zalazek/CMakeLists.txt`) pulls in all required sources, exposes the `inc/` directory to consumers, links against `dl` on Unix platforms, and writes the executable to the build tree root with an `rpath` pointing at the sibling `plugin/` directory. The plugin configuration (`zalazek/plugin/CMakeLists.txt`) loops over every module, compiling each as a CMake `MODULE` target with shared include paths and placing the resulting binaries in `build/plugin` so they are discoverable at runtime.

## Plugin Interface Update
`AbstractMobileObj.hh` now requires derived classes to implement `std::string GetStateDesc() const`. This pure-virtual method documents the need for plugins to expose a textual representation of the mobile object's state for the graphical server, ensuring all implementations satisfy the communication protocol.

