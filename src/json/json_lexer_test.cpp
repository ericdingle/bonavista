#include <cstring>
#include <tuple>
#include <vector>
#include "json/json_lexer.h"
#include "lexer/token_test_macros.h"
#include "util/status_test_macros.h"
#include "third_party/googletest/googletest/include/gtest/gtest.h"

class JsonLexerTest : public testing::Test {
 protected:
  JsonLexer lexer_;
};

TEST_F(JsonLexerTest, GetTokenUnexpected) {
  EXPECT_STATUS(lexer_.GetToken(".", 1, 1).status(),
                "Unexpected character: .", 1, 1);
  EXPECT_STATUS(lexer_.GetToken("blah", 1, 1).status(),
                "Unexpected character: b", 1, 1);
}

TEST_F(JsonLexerTest, GetTokenOperators) {
  std::vector<std::tuple<const char*, JsonLexer::Type>> test_cases = {
      std::make_tuple(":", JsonLexer::TYPE_COLON),
      std::make_tuple(",", JsonLexer::TYPE_COMMA),
      std::make_tuple("{", JsonLexer::TYPE_LEFT_BRACE),
      std::make_tuple("}", JsonLexer::TYPE_RIGHT_BRACE),
      std::make_tuple("[", JsonLexer::TYPE_LEFT_BRACKET),
      std::make_tuple("]", JsonLexer::TYPE_RIGHT_BRACKET),
      };

  for (const std::tuple<const char*, int>& test_case : test_cases) {
    const char* input = std::get<0>(test_case);
    EXPECT_TOKEN(*lexer_.GetToken(input, 1, 1).value(), std::get<1>(test_case),
                 input, 1, 1);
  }
}

TEST_F(JsonLexerTest, GetTokenKeyword) {
  std::vector<std::tuple<const char*, JsonLexer::Type>> test_cases = {
      std::make_tuple("false", JsonLexer::TYPE_FALSE),
      std::make_tuple("null", JsonLexer::TYPE_NULL),
      std::make_tuple("true", JsonLexer::TYPE_TRUE),
      };

  for (const std::tuple<const char*, JsonLexer::Type> test_case : test_cases) {
    const char* input = std::get<0>(test_case);
    EXPECT_TOKEN(*lexer_.GetToken(input, 1, 1).value(), std::get<1>(test_case),
                 input, 1, 1);
  }
}

TEST_F(JsonLexerTest, GetTokenNumber) {
  std::vector<const char*> test_cases = {
    "0", "-0", "1", "-1", "12", "123",
    "0.1", "-0.1", "12.3", "12.34",
    "5e3", "32E-1", "120e+012",
  };

  for (const char* test_case : test_cases) {
    EXPECT_TOKEN(*lexer_.GetToken(test_case, 1, 1).value(),
                 JsonLexer::TYPE_NUMBER, test_case, 1, 1);
  }
}

TEST_F(JsonLexerTest, GetTokenNumberError) {
  std::vector<const char*> test_cases = { "-", "1.", "23e", "35E+" };

  for (const char* test_case : test_cases) {
    EXPECT_STATUS(lexer_.GetToken(test_case, 1, 1).status(),
                  "Unexpected end of input", 1, 1);
  }
}

TEST_F(JsonLexerTest, GetTokenString) {
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
    const char* input = std::get<0>(test_case);
    std::unique_ptr<Token> token = lexer_.GetToken(input, 1, 1).value();
    EXPECT_TOKEN(*token, JsonLexer::TYPE_STRING, std::get<1>(test_case), 1, 1);
    EXPECT_EQ(strlen(input), token->length());
  }
}

TEST_F(JsonLexerTest, GetTokenStringError) {
  std::vector<std::tuple<const char*, const char*>> test_cases = {
    std::make_tuple("\"test", "Unexpected end of input"),
    std::make_tuple("\"test\n", "Unexpected character: \n"),
    std::make_tuple("\"test\\", "Unexpected end of input"),
    std::make_tuple("\"test\\a", "Unexpected character: a"),
    std::make_tuple("\"test\\u12", "Unexpected end of input"),
    std::make_tuple("\"test\\u123e", "Unexpected character: e"),
  };

  for (const std::tuple<const char*, const char*>& test_case : test_cases) {
    EXPECT_STATUS(lexer_.GetToken(std::get<0>(test_case), 1, 1).status(),
                  std::get<1>(test_case), 1, 1);
  }
}
