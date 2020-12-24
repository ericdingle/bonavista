#include "lexer/token.h"

#include "lexer/token_test_macros.h"
#include "third_party/googletest/googletest/include/gtest/gtest.h"

TEST(TokenTest, Constructor) {
  Token token(1, "lazy4", 12, 54);
  EXPECT_TOKEN(token, 1, "lazy4", 12, 54);
  EXPECT_TRUE(token.IsType(1));
  EXPECT_EQ(5, token.length());
}

TEST(TokenTest, Constructorlength) {
  Token token(1, "lazy4", 12, 54, 6);
  EXPECT_TOKEN(token, 1, "lazy4", 12, 54);
  EXPECT_TRUE(token.IsType(1));
  EXPECT_EQ(6, token.length());
}
