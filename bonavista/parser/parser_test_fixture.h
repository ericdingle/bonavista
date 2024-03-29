#ifndef PARSER_PARSER_TEST_FIXTURE_H_
#define PARSER_PARSER_TEST_FIXTURE_H_

#include <gtest/gtest.h>
#include <memory>

#include "absl/status/statusor.h"
#include "bonavista/lexer/token_stream.h"
#include "bonavista/parser/node.h"

template <typename LexerT, typename ParserT>
class ParserTestFixture : public testing::Test {
 protected:
  absl::StatusOr<std::unique_ptr<Node>> Parse(const char* input) {
    LexerT lexer;
    TokenStream stream(lexer, input);
    ParserT parser(&stream);
    return parser.Parse();
  }
};

#endif  // PARSER_PARSER_TEST_FIXTURE_H_
