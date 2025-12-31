cmake_minimum_required(VERSION 3.20)
project(csbuwq_launcher C)

function(add_launcher Name Dedicated)
    add_executable(${Name} WIN32
        Source/Launcher/Main.c
        Source/Launcher/Launcher.rc
    )

    target_compile_definitions(${Name} PRIVATE
        UNICODE
        _UNICODE
    )

    if (MSVC)
        target_link_options(${Name} PRIVATE /LARGEADDRESSAWARE /STACK:1572864)
    endif()

    if (Dedicated)
        target_compile_definitions(${Name} PRIVATE DEDICATED)
    endif()

    target_compile_options(${Name} PRIVATE
        /TC
        /O2
    )

    target_link_libraries(${Name} PRIVATE
        user32
        kernel32
    )

    set_target_properties(${Name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endfunction()

add_launcher(csbuwq OFF)
add_launcher(csbuwqds ON)
