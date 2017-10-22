add_library(status_lib status.cpp status.h status_or.h)
target_include_directories(status_lib INTERFACE ${bonavista_SOURCE_DIR})

if (bonavista_tests)
  add_executable(status_test status_test.cpp status_or_test.cpp)
  target_link_libraries(status_test gtest_main status_lib)
  add_test(NAME status_test COMMAND status_test)
endif()
