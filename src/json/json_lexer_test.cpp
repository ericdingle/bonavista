#include <cstring>
#include <utility>
#include "json/json_lexer.h"
#include "lexer/token_test_macros.h"
#include "util/status_test_macros.h"
#include "third_party/googletest/googletest/include/gtest/gtest.h"

class JsonLexerTest : public testing::Test {
 protected:
  Token GetToken(const char* input) {
    return *lexer_.GetToken(input, 1, 1).value();
  }

  Status GetStatus(const char* input) {
    return lexer_.GetToken(input, 1, 1).status();
  }

  JsonLexer lexer_;
};

TEST_F(JsonLexerTest, GetTokenUnexpected) {
  EXPECT_STATUS(GetStatus("."), "Unexpected character: .", 1, 1);
  EXPECT_STATUS(GetStatus("blah"), "Unexpected character: b", 1, 1);
}

TEST_F(JsonLexerTest, GetTokenOperators) {
  std::pair<const char*, JsonLexer::Type> test_cases[] = {
      {":", JsonLexer::TYPE_COLON},
      {",", JsonLexer::TYPE_COMMA},
      {"{", JsonLexer::TYPE_LEFT_BRACE},
      {"}", JsonLexer::TYPE_RIGHT_BRACE},
      {"[", JsonLexer::TYPE_LEFT_BRACKET},
      {"]", JsonLexer::TYPE_RIGHT_BRACKET},
      };

  for (const auto& test_case : test_cases) {
    const char* input = test_case.first;
    EXPECT_TOKEN(GetToken(input), test_case.second, input, 1, 1);
  }
}

TEST_F(JsonLexerTest, GetTokenKeyword) {
  const char* test_cases[] = {"false", "null", "true"};

  for (const char* test_case : test_cases) {
    EXPECT_TOKEN(GetToken(test_case), JsonLexer::TYPE_KEYWORD, test_case, 1, 1);
  }
}

TEST_F(JsonLexerTest, GetTokenNumber) {
  const char* test_cases[] = {
    "0", "-0", "1", "-1", "12", "123",
    "0.1", "-0.1", "12.3", "12.34",
    "5e3", "32E-1", "120e+012",
  };

  for (const char* test_case : test_cases) {
    EXPECT_TOKEN(GetToken(test_case), JsonLexer::TYPE_NUMBER, test_case, 1, 1);
  }
}

TEST_F(JsonLexerTest, GetTokenNumberError) {
  const char* test_cases[] = { "-", "1.", "23e", "35E+" };

  for (const char* test_case : test_cases) {
    EXPECT_STATUS(GetStatus(test_case), "Unexpected end of input", 1, 1);
  }
}

TEST_F(JsonLexerTest, GetTokenString) {
  std::pair<const char*, const char*> test_cases[] = {
      {"\"test\"", "test"},
      {"\"asdf jkl;\"", "asdf jkl;"},
      {"\"test\\b\"", "test\b"},
      {"\"test\\f\"", "test\f"},
      {"\"test\\n\"", "test\n"},
      {"\"test\\r\"", "test\r"},
      {"\"test\\t\"", "test\t"},
      {"\"test\\u1234\"", "test?"},
      {"\"test\\\\\"", "test\\"},
      {"\"test\\/\"", "test/"},
      {"\"test\\\"\"", "test\""},
  };

  for (const auto& test_case : test_cases) {
    const char* input = test_case.first;
    std::unique_ptr<Token> token = lexer_.GetToken(input, 1, 1).value();
    EXPECT_TOKEN(*token, JsonLexer::TYPE_STRING, test_case.second, 1, 1);
    EXPECT_EQ(strlen(input), token->length());
  }
}

TEST_F(JsonLexerTest, GetTokenStringError) {
  std::pair<const char*, const char*> test_cases[] = {
    {"\"test", "Unexpected end of input"},
    {"\"test\n", "Unexpected character: \n"},
    {"\"test\\", "Unexpected end of input"},
    {"\"test\\a", "Unexpected character: a"},
    {"\"test\\u12", "Unexpected end of input"},
    {"\"test\\u123e", "Unexpected character: e"},
  };

  for (const auto& test_case : test_cases) {
    EXPECT_STATUS(GetStatus(test_case.first), test_case.second, 1, 1);
  }
}
