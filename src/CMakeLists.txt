cmake_minimum_required (VERSION 2.8.2)

project(bonavista)

option(bonavista_tests "Whether to create test targets." ON)
if (bonavista_tests)
  enable_testing()

  # GoogleTest.
  configure_file(GoogleTest.cmake third_party/googletest/CMakeLists.txt)
  execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/third_party/googletest")
  execute_process(COMMAND "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/third_party/googletest")
  set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
  set(BUILD_GTEST ON CACHE BOOL "" FORCE)
  add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/third_party/googletest")
endif()

# C++11.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# Include relative to current source dir.
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(json EXCLUDE_FROM_ALL)
add_subdirectory(lexer)
add_subdirectory(parser)
