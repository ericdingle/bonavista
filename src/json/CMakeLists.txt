add_library(json_lib json_lexer.cpp json_parser.cpp)
target_link_libraries(json_lib lexer_lib parser_lib)

add_executable(json main.cpp)
target_link_libraries(json json_lib)

if (bonavista_tests)
  add_executable(json_test json_lexer_test.cpp json_parser_test.cpp)
  target_link_libraries(json_test gtest_main json_lib)
  add_test(NAME json_test COMMAND json_test)
endif()
