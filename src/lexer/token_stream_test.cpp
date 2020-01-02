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
      return std::make_unique<Token>(TYPE_CHAR, std::string(1, input[0]), line,
                                     column);
    }
  }
};

class TokenStreamTest : public testing::Test {
 protected:
  Token GetNextToken(TokenStream* token_stream) {
    return *token_stream->GetNextToken().value();
  }
};

TEST_F(TokenStreamTest, GetNextToken) {
  TestLexer lexer;
  TokenStream token_stream(&lexer, "a bc\nde  ");
  EXPECT_TRUE(token_stream.HasInput());

  EXPECT_TOKEN(GetNextToken(&token_stream), TestLexer::TYPE_CHAR, "a", 1, 1);
  EXPECT_TOKEN(GetNextToken(&token_stream), TestLexer::TYPE_CHAR, "b", 1, 3);
  EXPECT_TOKEN(GetNextToken(&token_stream), TestLexer::TYPE_CHAR, "c", 1, 4);
  EXPECT_TOKEN(GetNextToken(&token_stream), TestLexer::TYPE_CHAR, "d", 2, 1);
  EXPECT_TOKEN(GetNextToken(&token_stream), TestLexer::TYPE_CHAR, "e", 2, 2);
  EXPECT_TOKEN(GetNextToken(&token_stream), TestLexer::TYPE_END_OF_INPUT,
               "(end of input)", 2, 5);
  EXPECT_TOKEN(GetNextToken(&token_stream), TestLexer::TYPE_END_OF_INPUT,
               "(end of input)", 2, 5);

  EXPECT_FALSE(token_stream.HasInput());
}

TEST_F(TokenStreamTest, GetNextTokenError) {
  TestLexer lexer;
  TokenStream token_stream(&lexer, "f");
  EXPECT_TRUE(token_stream.HasInput());
  EXPECT_STATUS(token_stream.GetNextToken().status(), "fail", 1, 1);
}
