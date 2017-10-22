add_library(util_lib status.cpp status.h status_or.h)
target_include_directories(util_lib INTERFACE ${bonavista_SOURCE_DIR})

if (bonavista_tests)
  add_executable(util_test status_test.cpp status_or_test.cpp)
  target_link_libraries(util_test gtest_main util_lib)
  add_test(NAME util_test COMMAND util_test)
endif()
