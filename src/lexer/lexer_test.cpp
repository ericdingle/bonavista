#include "lexer/lexer.h"
#include "util/status_test_macros.h"
#include "third_party/googletest/googletest/include/gtest/gtest.h"

TEST(LexerTest, IsAlpha) {
  EXPECT_TRUE(Lexer::IsAlpha('a'));
  EXPECT_TRUE(Lexer::IsAlpha('n'));
  EXPECT_TRUE(Lexer::IsAlpha('z'));
  EXPECT_TRUE(Lexer::IsAlpha('A'));
  EXPECT_TRUE(Lexer::IsAlpha('N'));
  EXPECT_TRUE(Lexer::IsAlpha('Z'));
  EXPECT_FALSE(Lexer::IsAlpha('1'));
  EXPECT_FALSE(Lexer::IsAlpha(' '));
  EXPECT_FALSE(Lexer::IsAlpha('\0'));
}

TEST(LexerTest, IsDigit) {
  EXPECT_TRUE(Lexer::IsDigit('0'));
  EXPECT_TRUE(Lexer::IsDigit('5'));
  EXPECT_TRUE(Lexer::IsDigit('9'));
  EXPECT_FALSE(Lexer::IsDigit('a'));
  EXPECT_FALSE(Lexer::IsDigit(' '));
  EXPECT_FALSE(Lexer::IsDigit('\0'));
}

TEST(LexerTest, ExpectDigit) {
  EXPECT_OK(Lexer::ExpectDigit('0', 1, 2));
  EXPECT_STATUS(Lexer::ExpectDigit('a', 1, 2), "Unexpected character: a", 1, 2);
  EXPECT_STATUS(Lexer::ExpectDigit('\0', 1, 2), "Unexpected end of input", 1, 2);
}

TEST(LexerTest, ExpectNotControl) {
  EXPECT_OK(Lexer::ExpectNotControl('0', 1, 2));
  EXPECT_STATUS(Lexer::ExpectNotControl('\n', 1, 2), "Unexpected character: \n", 1, 2);
  EXPECT_STATUS(Lexer::ExpectNotControl('\0', 1, 2), "Unexpected end of input", 1, 2);
}

TEST(LexerTest, ExpectNotNull) {
  EXPECT_OK(Lexer::ExpectNotControl('0', 1, 2));
  EXPECT_STATUS(Lexer::ExpectNotControl('\0', 1, 2), "Unexpected end of input", 1, 2);
}

TEST(LexerTest, UnexpectedCharacter) {
  EXPECT_STATUS(Lexer::UnexpectedCharacter('0', 1, 2), "Unexpected character: 0", 1, 2);
}
