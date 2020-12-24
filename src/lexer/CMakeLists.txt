add_library(lexer_lib lexer.cpp lexer.h token.cpp token.h token_stream.cpp token_stream.h)
target_link_libraries(lexer_lib absl::status absl::statusor absl::strings)
target_include_directories(lexer_lib INTERFACE ${bonavista_SOURCE_DIR})

if (bonavista_tests)
  add_executable(lexer_test lexer_test.cpp token_stream_test.cpp token_test.cpp)
  target_link_libraries(lexer_test gtest_main lexer_lib)
  add_test(NAME lexer_test COMMAND lexer_test)
endif()
