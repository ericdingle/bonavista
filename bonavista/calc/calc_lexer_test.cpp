#include "bonavista/calc/calc_lexer.h"
#include "bonavista/lexer/lexer_test_fixture.h"
#include "bonavista/lexer/token_test_macros.h"

class CalcLexerTest : public LexerTestFixture<CalcLexer> {
};

TEST_F(CalcLexerTest, GetTokenUnexpected) {
  EXPECT_EQ(GetToken(".").status().message(),
            "Unexpected character '.' at 1:2.");
}

TEST_F(CalcLexerTest, GetTokenOperator) {
  std::pair<const char*, CalcLexer::Type> test_cases[] = {
      {"*", CalcLexer::TYPE_ASTERISK},
      {"(", CalcLexer::TYPE_LEFT_PARENTHESIS},
      {"-", CalcLexer::TYPE_MINUS},
      {"+", CalcLexer::TYPE_PLUS},
      {")", CalcLexer::TYPE_RIGHT_PARENTHESIS},
      {"/", CalcLexer::TYPE_SLASH},
      };

  for (const auto& test_case : test_cases) {
    const char* input = test_case.first;
    EXPECT_TOKEN(*GetToken(input).value(), test_case.second, input, 1, 2);
  }
}

TEST_F(CalcLexerTest, GetTokenNumber) {
  const char* test_cases[] = { "0", "1", "12", "123", "0.1", "12.3", "12.34" };

  for (const char* test_case : test_cases) {
    EXPECT_TOKEN(*GetToken(test_case).value(), CalcLexer::TYPE_NUMBER,
                 test_case, 1, 2);
  }
}

TEST_F(CalcLexerTest, GetTokenNumberError) {
  const char* test_cases[] = { "1." };

  for (const char* test_case : test_cases) {
    EXPECT_EQ(GetToken(test_case).status().message(),
              "Unexpected character '(end of input)' at 1:2.");
  }
}
