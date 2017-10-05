cmake_minimum_required (VERSION 2.8.2)

project(bonavista)

# C++11.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# Include relative to root source dir.
include_directories(${CMAKE_SOURCE_DIR})

option(bonavista_tests "Whether to create test targets." ON)
if (bonavista_tests)
  enable_testing()

  # GoogleTest.
  configure_file(GoogleTest.cmake third_party/googletest/CMakeLists.txt)
  execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/third_party/googletest")
  execute_process(COMMAND "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/third_party/googletest")
  add_subdirectory("${CMAKE_SOURCE_DIR}/third_party/googletest")
endif()

add_subdirectory(json)
add_subdirectory(lexer)
add_subdirectory(parser)
