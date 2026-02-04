cmake_minimum_required(VERSION 3.20)

project(raig_testbed
  VERSION 0.1.0
  LANGUAGES CXX
)

# --- Global settings ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(RAIG_ENABLE_TESTS "Build tests" ON)
option(RAIG_ENABLE_WARNINGS_AS_ERRORS "Treat warnings as errors" ON)

# --- Warnings (strict by default) ---
add_library(raig_warnings INTERFACE)
if (MSVC)
  target_compile_options(raig_warnings INTERFACE /W4)
  if (RAIG_ENABLE_WARNINGS_AS_ERRORS)
    target_compile_options(raig_warnings INTERFACE /WX)
  endif()
else()
  target_compile_options(raig_warnings INTERFACE
    -Wall -Wextra -Wpedantic
    -Wshadow -Wconversion -Wsign-conversion
    -Wnon-virtual-dtor -Wold-style-cast
    -Woverloaded-virtual -Wnull-dereference
  )
  if (RAIG_ENABLE_WARNINGS_AS_ERRORS)
    target_compile_options(raig_warnings INTERFACE -Werror)
  endif()
endif()

# --- Core library ---
add_library(raig_core STATIC
  src/genesis_core/genesis_core.cpp
  src/genesis_core/structural_state.cpp
  src/genesis_core/evolution_operator.cpp

  src/world/world_interface.cpp
  src/world/world_state.cpp
  src/world/stimulus_generators.cpp

  src/telemetry/telemetry_emitter.cpp
  src/telemetry/serializers.cpp
  src/telemetry/log_rotation.cpp

  src/runtime/runtime_supervisor.cpp
  src/runtime/pacing.cpp
  src/runtime/termination.cpp
)

target_include_directories(raig_core
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(raig_core
  PUBLIC raig_warnings
)

# (Later we will pin strict FP/UB policies here; keep minimal for skeleton)

# --- Executable (single instance runner) ---
add_executable(raig_testbed
  src/main.cpp
)

target_compile_options(raig_testbed PRIVATE -Wno-error)

target_link_libraries(raig_testbed
  PRIVATE raig_core
)

option(RAIG_ENABLE_TESTS "Enable RAIG tests" ON)

if(RAIG_ENABLE_TESTS)
  enable_testing()
  add_executable(raig_tests tests/raig_tests.cpp)
  target_link_libraries(raig_tests PRIVATE raig_core)
  add_test(NAME raig_tests COMMAND raig_tests)
endif()
