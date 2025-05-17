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

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|MSVC")
    target_compile_options(cursiobfuscator PRIVATE -Wall -Wextra -O2)
endif()