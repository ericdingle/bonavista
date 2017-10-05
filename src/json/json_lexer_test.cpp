#include "json/json_lexer.h"

#include <tuple>
#include <vector>
#include "third_party/googletest/googletest/include/gtest/gtest.h"

class JsonLexerTest : public testing::Test {
 protected:
  JsonLexer lexer_;

  std::string input_;
  int type_;
  std::string value_;
  int count_;
  std::string error_;
};

TEST_F(JsonLexerTest, TokenizeUnknown) {
  input_ = "blah";
  EXPECT_FALSE(lexer_.GetToken(input_, 0, &type_, &value_, &count_, &error_));
  EXPECT_FALSE(error_.empty());
}

TEST_F(JsonLexerTest, TokenizeOperators) {
  std::vector<std::tuple<const char*, JsonLexer::Type>> test_cases = {
      std::make_tuple(":", JsonLexer::TYPE_COLON),
      std::make_tuple(",", JsonLexer::TYPE_COMMA),
      std::make_tuple("{", JsonLexer::TYPE_LEFT_BRACE),
      std::make_tuple("}", JsonLexer::TYPE_RIGHT_BRACE),
      std::make_tuple("[", JsonLexer::TYPE_LEFT_BRACKET),
      std::make_tuple("]", JsonLexer::TYPE_RIGHT_BRACKET),
      };

  for (const std::tuple<const char*, int>& test_case : test_cases) {
    input_ = std::get<0>(test_case);
    EXPECT_TRUE(lexer_.GetToken(input_, 0, &type_, &value_, &count_, &error_));
    EXPECT_EQ(std::get<1>(test_case), type_);
    EXPECT_EQ(input_, value_);
    EXPECT_EQ(1, count_);
  }

  input_ = ".";
  EXPECT_FALSE(lexer_.GetToken(input_, 0, &type_, &value_, &count_, &error_));
  EXPECT_FALSE(error_.empty());
}

TEST_F(JsonLexerTest, TokenizeKeyword) {
  std::vector<std::tuple<const char*, JsonLexer::Type>> test_cases = {
      std::make_tuple("false", JsonLexer::TYPE_FALSE),
      std::make_tuple("null", JsonLexer::TYPE_NULL),
      std::make_tuple("true", JsonLexer::TYPE_TRUE),
      };

  for (const std::tuple<const char*, JsonLexer::Type> test_case : test_cases) {
    input_ = std::get<0>(test_case);
    EXPECT_TRUE(lexer_.GetToken(input_, 0, &type_, &value_, &count_, &error_));
    EXPECT_EQ(std::get<1>(test_case), type_);
    EXPECT_EQ(input_, value_);
    EXPECT_EQ(input_.length(), count_);
  }
}

TEST_F(JsonLexerTest, TokenizeNumber) {
  std::vector<const char*> test_cases = {
    "0", "-0", "1", "-1", "12", "123",
    "0.1", "-0.1", "12.3", "12.34",
    "5e3", "32E-1", "120e+012",
  };

  for (const char* test_case : test_cases) {
    input_ = test_case;
    EXPECT_TRUE(lexer_.GetToken(input_, 0, &type_, &value_, &count_, &error_));
    EXPECT_EQ(JsonLexer::TYPE_NUMBER, type_);
    EXPECT_EQ(input_, value_);
    EXPECT_EQ(input_.length(), count_);
  }
}

TEST_F(JsonLexerTest, TokenizeNumberError) {
  std::vector<const char*> test_cases = { "-", "01", "1.", "23e", "35E+" };

  for (const char* test_case : test_cases) {
    input_ = test_case;
    error_.clear();
    EXPECT_FALSE(lexer_.GetToken(input_, 0, &type_, &value_, &count_, &error_));
    EXPECT_FALSE(error_.empty());
  }
}

TEST_F(JsonLexerTest, TokenizeString) {
  std::vector<std::tuple<const char*, const char*>> test_cases = {
      std::make_tuple("\"test\"", "test"),
      std::make_tuple("\"asdf jkl;\"", "asdf jkl;"),
      std::make_tuple("\"test\\b\"", "test\b"),
      std::make_tuple("\"test\\f\"", "test\f"),
      std::make_tuple("\"test\\n\"", "test\n"),
      std::make_tuple("\"test\\r\"", "test\r"),
      std::make_tuple("\"test\\t\"", "test\t"),
      std::make_tuple("\"test\\u1234\"", "test?"),
      std::make_tuple("\"test\\\\\"", "test\\"),
      std::make_tuple("\"test\\/\"", "test/"),
      std::make_tuple("\"test\\\"\"", "test\""),
  };

  for (const std::tuple<const char*, const char*>& test_case : test_cases) {
    input_ = std::get<0>(test_case);
    EXPECT_TRUE(lexer_.GetToken(input_, 0, &type_, &value_, &count_, &error_));
    EXPECT_EQ(JsonLexer::TYPE_STRING, type_);
    EXPECT_EQ(std::get<1>(test_case), value_);
    EXPECT_EQ(input_.length(), count_);
  }
}

TEST_F(JsonLexerTest, TokenizeStringError) {
  std::vector<const char*> test_cases = {
    "\"test",
    "\"test\n",
    "\"test\\",
    "\"test\\a",
    "\"test\\u12",
    "\"test\\u123e",
  };

  for (const char* test_case : test_cases) {
    input_ = test_case;
    error_.clear();
    EXPECT_FALSE(lexer_.GetToken(input_, 0, &type_, &value_, &count_, &error_));
    EXPECT_FALSE(error_.empty());
  }
}
