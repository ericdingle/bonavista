#include "lexer/lexer.h"

#include <gtest/gtest.h>

#include "util/status_macros.h"

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
  EXPECT_EQ(Lexer::ExpectDigit('a', 1, 2).message(),
            "Unexpected character 'a' at 1:2.");
  EXPECT_EQ(Lexer::ExpectDigit('\0', 1, 2).message(),
            "Unexpected character '(end of input)' at 1:2.");
}

TEST(LexerTest, ExpectNotControl) {
  EXPECT_OK(Lexer::ExpectNotControl('0', 1, 2));
  EXPECT_EQ(Lexer::ExpectNotControl('\n', 1, 2).message(),
            "Unexpected character '\n' at 1:2.");
  EXPECT_EQ(Lexer::ExpectNotControl('\0', 1, 2).message(),
            "Unexpected character '(end of input)' at 1:2.");
}

TEST(LexerTest, ExpectNotNull) {
  EXPECT_OK(Lexer::ExpectNotControl('0', 1, 2));
  EXPECT_EQ(Lexer::ExpectNotControl('\0', 1, 2).message(),
            "Unexpected character '(end of input)' at 1:2.");
}
