#include "lexer/token.h"

#include "third_party/googletest/googletest/include/gtest/gtest.h"

TEST(TokenTest, Constructor) {
  Token token(1, "lazy4", 12, 54);
  EXPECT_TRUE(token.IsType(1));
  EXPECT_EQ(1, token.type());
  EXPECT_EQ("lazy4", token.value());
  EXPECT_EQ(12, token.line());
  EXPECT_EQ(54, token.column());
}
