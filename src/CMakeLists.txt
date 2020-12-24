cmake_minimum_required (VERSION 3.13.4)

project(bonavista)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include relative to current source dir.
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Abseil.
configure_file(Abseil.cmake third_party/absl/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/third_party/absl")
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/third_party/absl")
add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/absl EXCLUDE_FROM_ALL)
include_directories(${CMAKE_SOURCE_DIR}/third_party/absl)

option(bonavista_tests "Whether to create test targets." ON)
if (bonavista_tests)
  enable_testing()

  # GoogleTest.
  configure_file(GoogleTest.cmake third_party/googletest/CMakeLists.txt)
  execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/third_party/googletest")
  execute_process(COMMAND "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/third_party/googletest")
  set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
  set(BUILD_GTEST ON CACHE BOOL "" FORCE)
  add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/googletest EXCLUDE_FROM_ALL)
endif()

add_subdirectory(json)
add_subdirectory(lexer)
add_subdirectory(parser)
