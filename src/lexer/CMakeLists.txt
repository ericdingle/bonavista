add_library(lexer_lib lexer.cpp token.cpp token_stream.cpp)

if (bonavista_tests)
  add_executable(lexer_test token_stream_test.cpp token_test.cpp)
  target_link_libraries(lexer_test gtest_main lexer_lib)
  add_test(NAME lexer_test COMMAND lexer_test)
endif()
