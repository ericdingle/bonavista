cmake_minimum_required(VERSION 3.13.4)

project(abseil NONE)

include(ExternalProject)
ExternalProject_Add(abseil
  GIT_REPOSITORY    https://github.com/abseil/abseil-cpp.git
  GIT_TAG           master
  SOURCE_DIR        "${CMAKE_SOURCE_DIR}/third_party/absl"
  BINARY_DIR        ""
  CONFIGURE_COMMAND ""
  BUILD_COMMAND     ""
  INSTALL_COMMAND   ""
  TEST_COMMAND      ""
)
