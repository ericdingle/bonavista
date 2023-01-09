#include "json/json_lexer.h"

#include <utility>

#include "absl/strings/str_cat.h"
#include "lexer/lexer_test_fixture.h"
#include "lexer/token_test_macros.h"
#include "util/status_macros.h"

class JsonLexerTest : public LexerTestFixture<JsonLexer> {};

TEST_F(JsonLexerTest, GetTokenUnexpected) {
  EXPECT_EQ(GetToken(".").status().message(),
            "Unexpected character '.' at 1:2.");
  EXPECT_EQ(GetToken("blah").status().message(),
            "Unexpected character 'b' at 1:2.");
}

TEST_F(JsonLexerTest, GetTokenOperators) {
  std::pair<const char*, JsonLexer::Type> test_cases[] = {
      {":", JsonLexer::TYPE_COLON},        {",", JsonLexer::TYPE_COMMA},
      {"{", JsonLexer::TYPE_LEFT_BRACE},   {"}", JsonLexer::TYPE_RIGHT_BRACE},
      {"[", JsonLexer::TYPE_LEFT_BRACKET}, {"]", JsonLexer::TYPE_RIGHT_BRACKET},
  };

  for (const auto& test_case : test_cases) {
    const char* input = test_case.first;
    EXPECT_TOKEN(*GetToken(input).value(), test_case.second, input, 1, 2);
  }
}

TEST_F(JsonLexerTest, GetTokenKeyword) {
  const char* test_cases[] = {"false", "null", "true"};

  for (const char* test_case : test_cases) {
    EXPECT_TOKEN(*GetToken(test_case).value(), JsonLexer::TYPE_KEYWORD,
                 test_case, 1, 2);
  }
}

TEST_F(JsonLexerTest, GetTokenNumber) {
  const char* test_cases[] = {
      "0",    "-0",   "1",     "-1",  "12",    "123",      "0.1",
      "-0.1", "12.3", "12.34", "5e3", "32E-1", "120e+012",
  };

  for (const char* test_case : test_cases) {
    EXPECT_TOKEN(*GetToken(test_case).value(), JsonLexer::TYPE_NUMBER,
                 test_case, 1, 2);
  }
}

TEST_F(JsonLexerTest, GetTokenNumberError) {
  const char* test_cases[] = {"-", "1.", "23e", "35E+"};

  for (const char* test_case : test_cases) {
    EXPECT_EQ(GetToken(test_case).status().message(),
              "Unexpected character (end of input) at 1:2.");
  }
}

TEST_F(JsonLexerTest, GetTokenString) {
  std::pair<const char*, const char*> test_cases[] = {
      {"\"test\"", "test"},         {"\"asdf jkl;\"", "asdf jkl;"},
      {"\"test\\b\"", "test\\b"},   {"\"test\\f\"", "test\\f"},
      {"\"test\\n\"", "test\\n"},   {"\"test\\r\"", "test\\r"},
      {"\"test\\t\"", "test\\t"},   {"\"test\\u1234\"", "test\\u1234"},
      {"\"test\\\\\"", "test\\\\"}, {"\"test\\/\"", "test\\/"},
      {"\"test\\\"\"", "test\\\""},
  };

  for (const auto& test_case : test_cases) {
    std::unique_ptr<Token> token = GetToken(test_case.first).value();
    EXPECT_TOKEN(*token, JsonLexer::TYPE_STRING, test_case.second, 1, 2);
    EXPECT_EQ(static_cast<int>(strlen(test_case.first)), token->length());
  }
}

TEST_F(JsonLexerTest, GetTokenStringError) {
  std::pair<const char*, const char*> test_cases[] = {
      {"\"test", "Unexpected character (end of input)"},
      {"\"test\n", "Unexpected character '\n'"},
      {"\"test\\", "Unexpected character (end of input)"},
      {"\"test\\a", "Unexpected character 'a'"},
      {"\"test\\u12", "Unexpected character (end of input)"},
      {"\"test\\u123e", "Unexpected character 'e'"},
  };

  for (const auto& test_case : test_cases) {
    EXPECT_EQ(GetToken(test_case.first).status().message(),
              absl::StrCat(test_case.second, " at 1:2."));
  }
}
