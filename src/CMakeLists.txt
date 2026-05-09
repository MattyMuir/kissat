# LIBRARY PROJECT - kissat

# === Includes ===
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# === Create Library ===
add_library(kissat "allocate.c" "allocate.h" "analyze.c" "analyze.h" "ands.c" "ands.h" "application.c" "application.h" "arena.c" "arena.h" "array.h" "assign.c" "assign.h" "attribute.h" "averages.c" "averages.h" "backbone.c" "backbone.h" "backtrack.c" "backtrack.h" "build.c" "bump.c" "bump.h" "check.c" "check.h" "classify.c" "classify.h" "clause.c" "clause.h" "collect.c" "collect.h" "colors.c" "colors.h" "compact.c" "compact.h" "config.c" "config.h" "congruence.c" "congruence.h" "cover.h" "decide.c" "decide.h" "deduce.c" "deduce.h" "definition.c" "definition.h" "dense.c" "dense.h" "dump.c" "eliminate.c" "eliminate.h" "equivalences.c" "equivalences.h" "error.c" "error.h" "extend.c" "extend.h" "factor.c" "factor.h" "fastassign.h" "fastel.c" "fastel.h" "fifo.h" "file.c" "file.h" "flags.c" "flags.h" "format.c" "format.h" "forward.c" "forward.h" "frames.h" "gates.c" "gates.h" "handle.c" "handle.h" "heap.c" "heap.h" "ifthenelse.c" "ifthenelse.h" "import.c" "import.h" "inline.h" "inlineassign.h" "inlineframes.h" "inlineheap.h" "inlinequeue.h" "inlinevector.h" "internal.c" "internal.h" "keatures.h" "kimits.c" "kimits.h" "kissat.h" "kitten.c" "kitten.h" "krite.c" "krite.h" "learn.c" "learn.h" "literal.h" "logging.c" "logging.h" "lucky.c" "lucky.h" "main.c" "minimize.c" "minimize.h" "mode.c" "mode.h" "options.c" "options.h" "parse.c" "parse.h" "phases.c" "phases.h" "preprocess.c" "preprocess.h" "print.c" "print.h" "probe.c" "probe.h" "profile.c" "profile.h" "promote.c" "promote.h" "proof.c" "proof.h" "propbeyond.c" "propbeyond.h" "propdense.c" "propdense.h" "propinitially.c" "propinitially.h" "proplit.h" "proprobe.c" "proprobe.h" "propsearch.c" "propsearch.h" "queue.c" "queue.h" "random.h" "rank.h" "reduce.c" "reduce.h" "reference.h" "reluctant.c" "reluctant.h" "reorder.c" "reorder.h" "rephase.c" "rephase.h" "report.c" "report.h" "require.h" "resize.c" "resize.h" "resolve.c" "resolve.h" "resources.c" "resources.h" "restart.c" "restart.h" "search.c" "search.h" "shrink.c" "shrink.h" "smooth.c" "smooth.h" "sort.c" "sort.h" "stack.h" "statistics.c" "statistics.h" "strengthen.c" "strengthen.h" "substitute.c" "substitute.h" "sweep.c" "sweep.h" "terminate.c" "terminate.h" "tiers.c" "tiers.h" "trail.c" "trail.h" "transitive.c" "transitive.h" "utilities.c" "utilities.h" "value.h" "vector.c" "vector.h" "vivify.c" "vivify.h" "walk.c" "walk.h" "warmup.c" "warmup.h" "watch.c" "watch.h" "weaken.c" "weaken.h" "witness.c" "witness.h")

# === Link Libraries ===

# === Feature Enables ===
if (KS_MSVC_STATIC_RUNTIME)
    set_property(TARGET kissat PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
set_property(TARGET kissat PROPERTY CXX_STANDARD 20)
enable_ipo(kissat)
set_arch(kissat)

# === Install ===
# Install binaries and headers
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Config
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} # static library files
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} # includes
)

# Export includes
install(DIRECTORY "../include"
        DESTINATION ${CMAKE_INSTALL_PREFIX})
install(DIRECTORY "../src"
        DESTINATION ${CMAKE_INSTALL_PREFIX}
        FILES_MATCHING
        PATTERN "*.h")

# Configure the main config.cmake file
configure_package_config_file(
    ${CMAKE_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

# Install the export set and configuration files
install(EXPORT ${PROJECT_NAME}Config
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)