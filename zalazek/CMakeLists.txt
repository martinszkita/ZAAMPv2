set(ZALAZEK_PLUGIN_TARGETS
    Interp4Move
    Interp4Set
    Interp4Rotate
    Interp4Pause
)

find_package(XercesC 3.2 REQUIRED)

add_subdirectory(plugin)

set(CMDS_INTERPRETER_TARGET cmds_interpreter)
set(CMDS_INTERPRETER_SOURCES
    src/main.cpp
    src/ComChannel.cpp
    src/MobileObj.cpp
    src/Scene.cpp
    src/Sender.cpp
    src/xmlinterp.cpp
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

if(TARGET XercesC::XercesC)
    target_link_libraries(${CMDS_INTERPRETER_TARGET} PRIVATE XercesC::XercesC)
else()
    target_link_libraries(${CMDS_INTERPRETER_TARGET} PRIVATE ${XercesC_LIBRARIES})
    target_include_directories(${CMDS_INTERPRETER_TARGET} PRIVATE ${XercesC_INCLUDE_DIRS})
endif()

set_target_properties(${CMDS_INTERPRETER_TARGET}
    PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
        BUILD_RPATH "$ORIGIN/plugin"
)

add_dependencies(${CMDS_INTERPRETER_TARGET} ${ZALAZEK_PLUGIN_TARGETS})

set(ZALAZEK_CONFIG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/config)

add_custom_command(TARGET ${CMDS_INTERPRETER_TARGET} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E remove -f ${CMAKE_BINARY_DIR}/config
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${ZALAZEK_CONFIG_DIR} ${CMAKE_BINARY_DIR}/config
    COMMENT "Linking configuration directory into build tree"
)
