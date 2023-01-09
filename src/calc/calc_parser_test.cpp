#include "calc/calc_lexer.h"
#include "calc/calc_parser.h"
#include "lexer/token_test_macros.h"
#include "parser/parser_test_fixture.h"

class CalcParserTest : public ParserTestFixture<CalcLexer, CalcParser> {
};

TEST_F(CalcParserTest, ParseEmpty) {
  EXPECT_EQ(Parse("").status().message(),
            "Unexpected token '(end of input)' at 1:1.");
}

TEST_F(CalcParserTest, ParseMultiple) {
  EXPECT_EQ(Parse("1 2").status().message(), "Unexpected token '2' at 1:3.");
}

TEST_F(CalcParserTest, ParseUnknown) {
  EXPECT_EQ(Parse("b").status().message(), "Unexpected character 'b' at 1:1.");
}

TEST_F(CalcParserTest, ParseNumber) {
  EXPECT_TOKEN(Parse("1").value()->token(), CalcLexer::TYPE_NUMBER, "1", 1, 1);
}

TEST_F(CalcParserTest, ParseParenthesis) {
  EXPECT_TOKEN(Parse("(1)").value()->token(), CalcLexer::TYPE_NUMBER, "1", 1, 2);
}

TEST_F(CalcParserTest, ParseParenthesisError) {
  std::pair<const char*, const char*> test_cases[] = {
      {"( )", "Unexpected token ')' at 1:3."},
      {"(1", "Unexpected token '(end of input)' at 1:3."},
      {"( +)", "Unexpected token '+' at 1:3."},
      };

  for (const auto& test_case : test_cases) {
    EXPECT_EQ(Parse(test_case.first).status().message(), test_case.second);
  }
}

TEST_F(CalcParserTest, ParseOperator) {
  std::pair<const char*, CalcLexer::Type> test_cases[] = {
      {"*", CalcLexer::TYPE_ASTERISK},
      {"-", CalcLexer::TYPE_MINUS},
      {"+", CalcLexer::TYPE_PLUS},
      {"/", CalcLexer::TYPE_SLASH},
      };

  for (const auto& test_case : test_cases) {
    std::string input = std::string("1") + test_case.first + "2";

    auto node = Parse(input.c_str()).value();
    EXPECT_TOKEN(node->token(), test_case.second, test_case.first, 1, 2);
    EXPECT_EQ(2U, node->children().size());

    const auto& children = node->children();
    EXPECT_TOKEN(children[0]->token(), CalcLexer::TYPE_NUMBER, "1", 1, 1);
    EXPECT_TOKEN(children[1]->token(), CalcLexer::TYPE_NUMBER, "2", 1, 3);
  }
}

TEST_F(CalcParserTest, ParseOperatorError) {
  EXPECT_EQ(Parse("1+*").status().message(), "Unexpected token '*' at 1:3.");
}

TEST_F(CalcParserTest, OperatorPrecedence) {
  std::pair<const char*, CalcLexer::Type> test_cases[] = {
     {"1+2-3", CalcLexer::TYPE_MINUS},
     {"1-2+3", CalcLexer::TYPE_PLUS},
     {"1*2/3", CalcLexer::TYPE_SLASH},
     {"1/2*3", CalcLexer::TYPE_ASTERISK},
     {"1*2+3", CalcLexer::TYPE_PLUS},
     {"1-2/3", CalcLexer::TYPE_MINUS},
     {"1*(2+3)", CalcLexer::TYPE_ASTERISK},
     };

  for (const auto& test_case : test_cases) {
    EXPECT_EQ(test_case.second, Parse(test_case.first).value()->token().type());
  }
}
