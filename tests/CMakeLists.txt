# EXECUTABLE PROJECT - tests

# === Create Executable ===
add_executable(tests "tests.cpp")

# === Libraries ===
target_link_libraries(tests PRIVATE kissat)
target_include_directories(tests PRIVATE "../include/")

# === Feature Enables ===
if (KS_MSVC_STATIC_RUNTIME)
	set_property(TARGET tests PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
set_property(TARGET tests PROPERTY CXX_STANDARD 20)
enable_ipo(tests)
set_arch(tests)