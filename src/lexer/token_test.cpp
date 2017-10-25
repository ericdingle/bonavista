#include "lexer/token.h"
#include "third_party/googletest/googletest/include/gtest/gtest.h"

TEST(TokenTest, ConstructorChar) {
  Token token(1, 'c', 12, 54);
  EXPECT_TRUE(token.IsType(1));
  EXPECT_EQ(1, token.type());
  EXPECT_EQ("c", token.value());
  EXPECT_EQ(12, token.line());
  EXPECT_EQ(54, token.column());
  EXPECT_EQ(1, token.length());
}

TEST(TokenTest, ConstructorString) {
  Token token(1, "lazy4", 12, 54);
  EXPECT_TRUE(token.IsType(1));
  EXPECT_EQ(1, token.type());
  EXPECT_EQ("lazy4", token.value());
  EXPECT_EQ(12, token.line());
  EXPECT_EQ(54, token.column());
  EXPECT_EQ(5, token.length());
}

TEST(TokenTest, Constructorlength) {
  Token token(1, "lazy4", 12, 54, 6);
  EXPECT_TRUE(token.IsType(1));
  EXPECT_EQ(1, token.type());
  EXPECT_EQ("lazy4", token.value());
  EXPECT_EQ(12, token.line());
  EXPECT_EQ(54, token.column());
  EXPECT_EQ(6, token.length());
}
