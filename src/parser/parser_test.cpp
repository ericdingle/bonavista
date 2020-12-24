#include "parser/parser.h"

#include "lexer/lexer.h"
#include "lexer/token_stream.h"
#include "lexer/token_test_macros.h"
#include "parser/node.h"
#include "parser/parser_test_fixture.h"
#include "util/status_macros.h"

class TestLexer : public Lexer {
 public:
  enum Type { TYPE_DIGIT, TYPE_PLUS };

  absl::StatusOr<std::unique_ptr<Token>> GetToken(std::string_view input,
                                                  int line,
                                                  int column) const override {
    if (input[0] == '+') {
      return std::make_unique<Token>(TYPE_PLUS, "+", line, column);
    } else if (IsDigit(input[0])) {
      return std::make_unique<Token>(TYPE_DIGIT, input.substr(0, 1), line,
                                     column);
    }
    return InvalidArgumentError(input[0], line, column);
  }
};

class TestParser : public Parser {
 public:
  using Parser::Parser;

 protected:
  int GetBindingPower(int type) const override {
    return type == TestLexer::TYPE_PLUS ? 10 : 0;
  }

  absl::StatusOr<std::unique_ptr<Node>> ParsePrefixToken(
      std::unique_ptr<const Token> token) override {
    if (token->IsType(TestLexer::TYPE_DIGIT)) {
      auto node = std::make_unique<Node>(std::move(token));

      // Weird behavior only for the test.
      if (node->token().value() == "0") {
        RETURN_IF_ERROR(ConsumeToken(TestLexer::TYPE_DIGIT));
      }

      return std::move(node);
    }
    return UnexpectedToken(*token);
  }

  absl::StatusOr<std::unique_ptr<Node>> ParseInfixToken(
      std::unique_ptr<const Token> token,
      std::unique_ptr<const Node> left) override {
    if (token->IsType(TestLexer::TYPE_PLUS)) {
      auto node = std::make_unique<Node>(std::move(token));
      node->AddChild(std::move(left));

      ASSIGN_OR_RETURN(auto right, ParseExpression(10));
      node->AddChild(std::move(right));

      return std::move(node);
    }
    return UnexpectedToken(*token);
  }
};

class ParserTest : public ParserTestFixture<TestLexer, TestParser> {};

TEST_F(ParserTest, ExpectToken) {
  Token token(1, "a", 2, 3);
  EXPECT_OK(Parser::ExpectToken(token, 1));
  EXPECT_EQ(Parser::ExpectToken(token, 2).message(),
            "Unexpected token 'a' at 2:3.");
}

TEST_F(ParserTest, UnexpectedToken) {
  Token token(1, "a", 2, 3);
  EXPECT_EQ(Parser::UnexpectedToken(token).message(),
            "Unexpected token 'a' at 2:3.");
}

TEST_F(ParserTest, Empty) {
  EXPECT_EQ(Parse("").status().message(),
            "Unexpected token '(end of input)' at 1:1.");
}

TEST_F(ParserTest, UnexpectedCharacter) {
  EXPECT_EQ(Parse("a").status().message(), "Unexpected character 'a' at 1:1.");
}

TEST_F(ParserTest, Prefix) {
  EXPECT_TOKEN(Parse("1").value()->token(), TestLexer::TYPE_DIGIT, "1", 1, 1);
}

TEST_F(ParserTest, PrefixError) {
  EXPECT_EQ(Parse("+").status().message(), "Unexpected token '+' at 1:1.");
}

TEST_F(ParserTest, Infix) {
  std::unique_ptr<Node> node = Parse("1+2").value();
  EXPECT_TOKEN(node->token(), TestLexer::TYPE_PLUS, "+", 1, 2);
  EXPECT_EQ(2U, node->children().size());
  EXPECT_TOKEN(node->children()[0]->token(), TestLexer::TYPE_DIGIT, "1", 1, 1);
  EXPECT_TOKEN(node->children()[1]->token(), TestLexer::TYPE_DIGIT, "2", 1, 3);
}

TEST_F(ParserTest, InfixError) {
  EXPECT_EQ(Parse("1+").status().message(),
            "Unexpected token '(end of input)' at 1:3.");
}

TEST_F(ParserTest, ConsumeToken) {
  std::unique_ptr<Node> node = Parse("01").value();
  EXPECT_TOKEN(node->token(), TestLexer::TYPE_DIGIT, "0", 1, 1);
  EXPECT_TRUE(node->children().empty());
}

TEST_F(ParserTest, ConsumeTokenError) {
  EXPECT_EQ(Parse("0").status().message(),
            "Unexpected token '(end of input)' at 1:2.");
}

TEST_F(ParserTest, ParseMultiple) {
  TestLexer lexer;
  TokenStream stream(lexer, "1 1 1");
  TestParser parser(&stream);

  for (int i = 0; i < 3; ++i) {
    std::unique_ptr<Node> node = parser.Parse().value();
    EXPECT_TOKEN(node->token(), TestLexer::TYPE_DIGIT, "1", 1, 2 * i + 1);
    EXPECT_TRUE(node->children().empty());
  }

  EXPECT_EQ(parser.Parse().status().message(),
            "Unexpected token '(end of input)' at 1:6.");
}
