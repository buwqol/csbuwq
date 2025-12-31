cmake_minimum_required(VERSION 3.20)

project(csbuwq_launcher C)

add_executable(csbuwq WIN32
    Source/Launcher/Main.c
    Source/Launcher/Launcher.rc
)

target_compile_definitions(csbuwq PRIVATE
    UNICODE
    _UNICODE
)

target_compile_options(csbuwq PRIVATE
    /TC
    /O2
)

target_link_libraries(csbuwq PRIVATE
    user32
    kernel32
)

target_link_options(csbuwq PRIVATE
    /LARGEADDRESSAWARE
    /STACK:1572864
)

set_target_properties(csbuwq PROPERTIES
    OUTPUT_NAME "csbuwq"
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)