cmake_minimum_required(VERSION 3.24)
project(Vypr VERSION 1.0 LANGUAGES C CXX)
enable_testing()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(TARGET_ARCH "${CMAKE_HOST_SYSTEM_PROCESSOR}" CACHE STRING "Architecture to build compiler for.")
set(ENABLE_COVERAGE false CACHE BOOL "Enable code coverage profiles.")

if (ENABLE_COVERAGE)
  message("Code coverage enabled.")
  include(CodeCoverage)
  append_coverage_compiler_flags()
endif (ENABLE_COVERAGE)

message("Setting compiler architecture to ${TARGET_ARCH}")

find_package(LLVM REQUIRED CONFIG)

add_definitions(${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(LLVM_LIBS ${LLVM_TARGETS_TO_BUILD})

add_subdirectory(Vypr)
add_subdirectory(VyprTest)
