add_library(parser_lib ast_node.cpp ast_node.h parser.cpp parser.h)
target_link_libraries(parser_lib lexer_lib)
target_include_directories(parser_lib INTERFACE ${bonavista_SOURCE_DIR})

if (bonavista_tests)
  add_executable(parser_test parser_test.cpp)
  target_link_libraries(parser_test gtest_main parser_lib)
  add_test(NAME parser_test COMMAND parser_test)
endif()
