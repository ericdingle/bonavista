#include "lexer/token.h"

#include "third_party/googletest/src/googletest/include/gtest/gtest.h"

TEST(TokenTest, Constructor) {
  Token::Position position;
  position.line = 12;
  position.column = 54;

  Token token(1, "lazy4", position);
  EXPECT_TRUE(token.IsType(1));
  EXPECT_EQ(1, token.type());
  EXPECT_EQ("lazy4", token.value());
  EXPECT_EQ(12, token.position().line);
  EXPECT_EQ(54, token.position().column);
}
