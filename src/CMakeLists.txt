cmake_minimum_required (VERSION 2.8.2)

project(bonavista)

# GoogleTest.
configure_file(GoogleTest.cmake third_party/googletest/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/third_party/googletest")
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/third_party/googletest")
# Prevent overriding the parent project's compiler/linker settings on Windows.
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# Add directly to our build. This adds following targets:
#   gtest, gtest_main, gmock, gmock_main
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/third_party/googletest")

# Use C++11.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# Allow including files relative to top-level and build directories.
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
# Enable testing.
enable_testing()

# Subdirectories.
add_subdirectory(json)
add_subdirectory(lexer)
add_subdirectory(parser)
