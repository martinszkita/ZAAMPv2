set(ZALAZEK_PLUGIN_TARGETS
    Interp4Move
    Interp4Set
    Interp4Rotate
    Interp4Pause
)

add_subdirectory(plugin)

set(CMDS_INTERPRETER_TARGET cmds_interpreter)
set(CMDS_INTERPRETER_SOURCES
    src/main.cpp
    src/ComChannel.cpp
    src/MobileObj.cpp
    src/Scene.cpp
    src/Sender.cpp
)

add_executable(${CMDS_INTERPRETER_TARGET}
    ${CMDS_INTERPRETER_SOURCES}
)

target_include_directories(${CMDS_INTERPRETER_TARGET}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_compile_features(${CMDS_INTERPRETER_TARGET} PRIVATE cxx_std_17)

if(UNIX AND NOT APPLE)
    target_link_libraries(${CMDS_INTERPRETER_TARGET} PRIVATE dl)
endif()

set_target_properties(${CMDS_INTERPRETER_TARGET}
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
        BUILD_RPATH "$ORIGIN/plugin"
)

add_dependencies(${CMDS_INTERPRETER_TARGET} ${ZALAZEK_PLUGIN_TARGETS})
