# GLOBAL - kissat

# === Includes ===
cmake_policy(SET CMP0069 NEW)
include(cmake/macros.cmake)

# === Configuration ===
cmake_minimum_required(VERSION 3.21)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable multi-processor build
enable_mp()

# Options
set(WARNINGS OFF CACHE BOOL "Enables clang-tidy and warnings on clang-like compilers")
if (WARNINGS)
	add_compile_options("-Wall")
    add_compile_options("-Wextra")
    add_compile_options("-Wpedantic")
    enable_clangtidy()
endif()

set(KS_NO_ITERATOR_DEBUGGING OFF CACHE BOOL "Disable iterator debugging")
if (KS_NO_ITERATOR_DEBUGGING)
    add_compile_definitions(_HAS_ITERATOR_DEBUGGING=0)
endif()

set(KS_MSVC_STATIC_RUNTIME, OFF CACHE BOOL "Use static C runtime library when taregting MSVC ABI")
if (KS_MSVC_STATIC_RUNTIME)
    use_static_msvc_crt()
endif()

# === Projects ===
project("kissat")
add_subdirectory("src")

add_subdirectory("examples")

# === Tests ===
enable_testing()
set(CTEST_OUTPUT_ON_FAILURE ON)

add_subdirectory("tests")

add_test(NAME Test7 COMMAND tests 0)
add_test(NAME Test12 COMMAND tests 1)