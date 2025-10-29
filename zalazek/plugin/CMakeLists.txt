cmake_minimum_required(VERSION 3.0)

set(PROJ_NAME "plugins_4_program")
project(${PROJ_NAME})

include_directories(${PROJECT_SOURCE_DIR}/inc ${PROJECT_SOURCE_DIR}/../inc)
add_library(Interp4Move MODULE src/Interp4Move.cpp)
add_custom_command(TARGET Interp4Move POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo " Utworzony zostal plugin:  libInterp4Move.so"
    COMMAND ${CMAKE_COMMAND} -E echo ""    
  )

add_library(Interp4Set MODULE src/Interp4Set.cpp)
add_custom_command(TARGET Interp4Set POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo " Utworzony zostal plugin:  libInterp4Set.so"
    COMMAND ${CMAKE_COMMAND} -E echo ""    
  )

add_library(Interp4Rotate MODULE src/Interp4Rotate.cpp)
add_custom_command(TARGET Interp4Rotate POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo " Utworzony zostal plugin:  InterpRotate.so"
    COMMAND ${CMAKE_COMMAND} -E echo ""    
  )

add_library(Interp4Pause MODULE src/Interp4Pause.cpp)
add_custom_command(TARGET Interp4Pause POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo ""
    COMMAND ${CMAKE_COMMAND} -E echo " Utworzony zostal plugin:  InterpPause.so"
    COMMAND ${CMAKE_COMMAND} -E echo ""    
  )

