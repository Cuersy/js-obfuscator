cmake_minimum_required(VERSION 3.10)
project(CursiObfuscator VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
include_directories(${CMAKE_SOURCE_DIR}/include)
set(SOURCES
    src/main.cc
    src/parser.cc
    src/lexer.cc
    src/obfuscator.cc
)
add_executable(cursiobfuscator ${SOURCES})
if (MSVC)
    message(STATUS "Bulding with MSVC detected. Configuring for Windows...")
    target_compile_options(cursiobfuscator PRIVATE
        "$<$<CONFIG:Debug>:/W4;/RTC1>"
        "$<$<CONFIG:Release>:/W4;/O2>"
    )
    target_compile_definitions(cursiobfuscator PRIVATE
        _UNICODE
        UNICODE
    )

else()
    message(STATUS "GCC/Clang detected. Configuring for Unix-like OS...")

    target_compile_options(cursiobfuscator PRIVATE
        "$<$<CONFIG:Debug>:-g;-Wall;-Wextra>"
        "$<$<CONFIG:Release>:-O2;-Wall;-Wextra>"
    )
endif()
set_target_properties(cursiobfuscator PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
