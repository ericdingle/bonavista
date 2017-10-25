#include "json/json_lexer.h"

#include <cstring>
#include <tuple>
#include <vector>
#include "third_party/googletest/googletest/include/gtest/gtest.h"

class JsonLexerTest : public testing::Test {
 protected:
  void ExpectStatus(const char* input, const std::string& message) {
    auto status_or = lexer_.GetToken(input, 1, 1);
    ASSERT_FALSE(status_or.ok()) << "Input: " << input;
    EXPECT_EQ(message, status_or.status().message()) << input;
  }

  void ExpectToken(const char* input, int type, const std::string& value,
                   int length=0) {
    auto status_or = lexer_.GetToken(input, 1, 1);
    ASSERT_TRUE(status_or.ok()) << "Input: " << input << ". Status: " <<
        status_or.status().ToString();
    const auto& token = status_or.value();
    EXPECT_EQ(type, token->type());
    EXPECT_EQ(value, token->value());
    if (length)
      EXPECT_EQ(length, token->length());
   }

  JsonLexer lexer_;
};

TEST_F(JsonLexerTest, GetTokenUnexpected) {
  ExpectStatus(".", "Unexpected character: .");
  ExpectStatus("blah", "Unexpected character: b");
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
    ExpectToken(input, std::get<1>(test_case), input);
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
    ExpectToken(input, std::get<1>(test_case), input);
  }
}

TEST_F(JsonLexerTest, GetTokenNumber) {
  std::vector<const char*> test_cases = {
    "0", "-0", "1", "-1", "12", "123",
    "0.1", "-0.1", "12.3", "12.34",
    "5e3", "32E-1", "120e+012",
  };

  for (const char* test_case : test_cases) {
    ExpectToken(test_case, JsonLexer::TYPE_NUMBER, test_case);
  }
}

TEST_F(JsonLexerTest, GetTokenNumberError) {
  std::vector<const char*> test_cases = { "-", "1.", "23e", "35E+" };

  for (const char* test_case : test_cases) {
    ExpectStatus(test_case, "Unexpected end of input");
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
    ExpectToken(input, JsonLexer::TYPE_STRING, std::get<1>(test_case),
                strlen(input));
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
    ExpectStatus(std::get<0>(test_case), std::get<1>(test_case));
  }
}
