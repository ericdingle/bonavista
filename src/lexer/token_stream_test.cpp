#include "lexer/token_stream.h"

#include <memory>
#include "lexer/lexer.h"
#include "third_party/googletest/googletest/include/gtest/gtest.h"

class TestLexer : public Lexer {
 public:
  enum Type {
    TYPE_CHAR
  };

  virtual bool GetToken(const std::string& input,
                        int index,
                        int* type,
                        std::string* value,
                        int* count,
                        std::string* error) const {
    *type = TYPE_CHAR;
    *value = input[index];
    *count = 1;
    return true;
  }
};

TEST(TokenStreamTest, GetNextToken) {
  TestLexer lexer;
  TokenStream token_stream(&lexer, "a bc\nde  ");
  EXPECT_TRUE(token_stream.HasInput());

  std::unique_ptr<const Token> token;
  EXPECT_TRUE(token_stream.GetNextToken(&token));
  EXPECT_TRUE(token->IsType(TestLexer::TYPE_CHAR));
  EXPECT_EQ("a", token->value());
  EXPECT_EQ(1, token->line());
  EXPECT_EQ(1, token->column());

  EXPECT_TRUE(token_stream.GetNextToken(&token));
  EXPECT_TRUE(token->IsType(TestLexer::TYPE_CHAR));
  EXPECT_EQ("b", token->value());
  EXPECT_EQ(1, token->line());
  EXPECT_EQ(3, token->column());

  EXPECT_TRUE(token_stream.GetNextToken(&token));
  EXPECT_TRUE(token->IsType(TestLexer::TYPE_CHAR));
  EXPECT_EQ("c", token->value());
  EXPECT_EQ(1, token->line());
  EXPECT_EQ(4, token->column());

  EXPECT_TRUE(token_stream.GetNextToken(&token));
  EXPECT_TRUE(token->IsType(TestLexer::TYPE_CHAR));
  EXPECT_EQ("d", token->value());
  EXPECT_EQ(2, token->line());
  EXPECT_EQ(1, token->column());

  EXPECT_TRUE(token_stream.GetNextToken(&token));
  EXPECT_TRUE(token->IsType(TestLexer::TYPE_CHAR));
  EXPECT_EQ("e", token->value());
  EXPECT_EQ(2, token->line());
  EXPECT_EQ(2, token->column());

  EXPECT_TRUE(token_stream.GetNextToken(&token));
  EXPECT_TRUE(token->IsType(TestLexer::TYPE_END_OF_INPUT));
  EXPECT_EQ("(end of input)", token->value());
  EXPECT_EQ(2, token->line());
  EXPECT_EQ(5, token->column());

  EXPECT_TRUE(token_stream.GetNextToken(&token));
  EXPECT_TRUE(token->IsType(TestLexer::TYPE_END_OF_INPUT));
  EXPECT_EQ("(end of input)", token->value());
  EXPECT_EQ(2, token->line());
  EXPECT_EQ(5, token->column());
  EXPECT_FALSE(token_stream.HasInput());
}
