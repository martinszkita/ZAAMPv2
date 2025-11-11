set(PLUGIN_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../inc
)

foreach(plugin_target IN LISTS ZALAZEK_PLUGIN_TARGETS)
    add_library(${plugin_target} SHARED src/${plugin_target}.cpp)
    target_include_directories(${plugin_target}
        PRIVATE
            ${PLUGIN_INCLUDE_DIRS}
    )
    target_compile_features(${plugin_target} PRIVATE cxx_std_17)
    set_target_properties(${plugin_target}
        PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugin
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugin
    )
endforeach()
