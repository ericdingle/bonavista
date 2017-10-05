add_library(parser_lib ast_node.cpp parser.cpp)
target_link_libraries(parser_lib lexer_lib)

add_executable(parser_test parser_test.cpp)
target_link_libraries(parser_test gtest_main parser_lib)
add_test(NAME parser_test COMMAND parser_test)
