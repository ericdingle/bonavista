#include "lexer/lexer.h"
#include "third_party/googletest/googletest/include/gtest/gtest.h"

class LexerTest : public testing::Test {
 protected:
  void ExpectStatus(const Status& status, const std::string& message) {
    EXPECT_EQ(message, status.message());
    if (!message.empty()) {
      EXPECT_EQ(1, status.line());
      EXPECT_EQ(2, status.column());
    }
  }
};

TEST_F(LexerTest, IsAlpha) {
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

TEST_F(LexerTest, IsDigit) {
  EXPECT_TRUE(Lexer::IsDigit('0'));
  EXPECT_TRUE(Lexer::IsDigit('5'));
  EXPECT_TRUE(Lexer::IsDigit('9'));
  EXPECT_FALSE(Lexer::IsDigit('a'));
  EXPECT_FALSE(Lexer::IsDigit(' '));
  EXPECT_FALSE(Lexer::IsDigit('\0'));
}

TEST_F(LexerTest, ExpectDigit) {
  ExpectStatus(Lexer::ExpectDigit('0', 1, 2), "");
  ExpectStatus(Lexer::ExpectDigit('a', 1, 2), "Unexpected character: a");
  ExpectStatus(Lexer::ExpectDigit('\0', 1, 2), "Unexpected end of input");
}

TEST_F(LexerTest, ExpectNotControl) {
  ExpectStatus(Lexer::ExpectNotControl('0', 1, 2), "");
  ExpectStatus(Lexer::ExpectNotControl('\n', 1, 2), "Unexpected character: \n");
  ExpectStatus(Lexer::ExpectNotControl('\0', 1, 2), "Unexpected end of input");
}

TEST_F(LexerTest, ExpectNotNull) {
  ExpectStatus(Lexer::ExpectNotControl('0', 1, 2), "");
  ExpectStatus(Lexer::ExpectNotControl('\0', 1, 2), "Unexpected end of input");
}

TEST_F(LexerTest, UnexpectedCharacter) {
  ExpectStatus(Lexer::UnexpectedCharacter('0', 1, 2), "Unexpected character: 0");
}
