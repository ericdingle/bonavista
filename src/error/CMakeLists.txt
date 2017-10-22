add_library(error_lib error.cpp error.h error_or.h)
target_include_directories(error_lib INTERFACE ${bonavista_SOURCE_DIR})

if (bonavista_tests)
  add_executable(error_test error_test.cpp error_or_test.cpp)
  target_link_libraries(error_test gtest_main error_lib)
  add_test(NAME error_test COMMAND error_test)
endif()
