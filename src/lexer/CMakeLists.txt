add_library(lexer_lib lexer.cpp lexer.h token.cpp token.h token_stream.cpp token_stream.h)

if (bonavista_tests)
  add_executable(lexer_test token_stream_test.cpp token_test.cpp)
  target_link_libraries(lexer_test gtest_main lexer_lib)
  add_test(NAME lexer_test COMMAND lexer_test)
endif()
