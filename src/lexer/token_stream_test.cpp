#include "lexer/token_stream.h"

#include <memory>
#include "lexer/lexer.h"
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

class TokenStreamTest : public testing::Test {
 protected:
  void ExpectToken(TokenStream* token_stream, int type, const std::string& value,
                   int line, int column) {
    auto status_or = token_stream->GetNextToken();
    EXPECT_TRUE(status_or.ok());
    auto token = status_or.value();
    EXPECT_EQ(type, token->type());
    EXPECT_EQ(value, token->value());
    EXPECT_EQ(line, token->line());
    EXPECT_EQ(column, token->column());
  }
};

TEST_F(TokenStreamTest, GetNextToken) {
  TestLexer lexer;
  TokenStream token_stream(&lexer, "a bc\nde  ");
  EXPECT_TRUE(token_stream.HasInput());

  ExpectToken(&token_stream, TestLexer::TYPE_CHAR, "a", 1, 1);
  ExpectToken(&token_stream, TestLexer::TYPE_CHAR, "b", 1, 3);
  ExpectToken(&token_stream, TestLexer::TYPE_CHAR, "c", 1, 4);
  ExpectToken(&token_stream, TestLexer::TYPE_CHAR, "d", 2, 1);
  ExpectToken(&token_stream, TestLexer::TYPE_CHAR, "e", 2, 2);
  ExpectToken(&token_stream, TestLexer::TYPE_END_OF_INPUT,
              "(end of input)", 2, 5);
  ExpectToken(&token_stream, TestLexer::TYPE_END_OF_INPUT,
              "(end of input)", 2, 5);

  EXPECT_FALSE(token_stream.HasInput());
}

TEST_F(TokenStreamTest, GetNextTokenError) {
  TestLexer lexer;
  TokenStream token_stream(&lexer, "f");
  EXPECT_TRUE(token_stream.HasInput());

  auto status_or = token_stream.GetNextToken();
  EXPECT_FALSE(status_or.ok());
  EXPECT_EQ("fail", status_or.status().message());
}
