#include "lexer/token_stream.h"

#include <memory>
#include "lexer/lexer.h"
#include "lexer/token_test_macros.h"
#include "util/status_test_macros.h"
#include "third_party/googletest/googletest/include/gtest/gtest.h"

class TestLexer : public Lexer {
 public:
  enum Type {
    TYPE_CHAR
  };

  StatusOr<std::unique_ptr<Token>> GetToken(
      const char* input, int line, int column) const override {
    if (input[0] == 'f') {
      return Status("fail", line, column);
    } else {
      return std::unique_ptr<Token>(
          new Token(TYPE_CHAR, std::string(1, input[0]), line, column));
    }
  }
};

TEST(TokenStreamTest, GetNextToken) {
  TestLexer lexer;
  TokenStream token_stream(&lexer, "a bc\nde  ");
  EXPECT_TRUE(token_stream.HasInput());

  EXPECT_TOKEN(*token_stream.GetNextToken().value(), TestLexer::TYPE_CHAR,
               "a", 1, 1);
  EXPECT_TOKEN(*token_stream.GetNextToken().value(), TestLexer::TYPE_CHAR,
               "b", 1, 3);
  EXPECT_TOKEN(*token_stream.GetNextToken().value(), TestLexer::TYPE_CHAR,
               "c", 1, 4);
  EXPECT_TOKEN(*token_stream.GetNextToken().value(), TestLexer::TYPE_CHAR,
               "d", 2, 1);
  EXPECT_TOKEN(*token_stream.GetNextToken().value(), TestLexer::TYPE_CHAR,
               "e", 2, 2);
  EXPECT_TOKEN(*token_stream.GetNextToken().value(),
               TestLexer::TYPE_END_OF_INPUT, "(end of input)", 2, 5);
  EXPECT_TOKEN(*token_stream.GetNextToken().value(),
               TestLexer::TYPE_END_OF_INPUT, "(end of input)", 2, 5);

  EXPECT_FALSE(token_stream.HasInput());
}

TEST(TokenStreamTest, GetNextTokenError) {
  TestLexer lexer;
  TokenStream token_stream(&lexer, "f");
  EXPECT_TRUE(token_stream.HasInput());
  EXPECT_EQ("fail", token_stream.GetNextToken().status().message());
}
