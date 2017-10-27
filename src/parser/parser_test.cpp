#include "parser/parser.h"

#include "lexer/lexer.h"
#include "lexer/token_stream.h"
#include "parser/node.h"
#include "util/status_test_macros.h"
#include "third_party/googletest/googletest/include/gtest/gtest.h"

class TestLexer : public Lexer {
 public:
  enum Type {
    TYPE_DIGIT,
    TYPE_PLUS
  };

  StatusOr<std::unique_ptr<Token>> GetToken(
      const char* input, int line, int column) const override {
    if (*input == '+') {
      return std::unique_ptr<Token>(new Token(TYPE_PLUS, "+", line, column));
    } else if (IsDigit(*input)) {
      return std::unique_ptr<Token>(new Token(TYPE_DIGIT, std::string(1, *input),
                                              line, column));
    }
    return UnexpectedCharacter(*input, line, column);
  }
};

class TestParser : public Parser {
 public:
  using Parser::Parser;

 protected:
  int GetBindingPower(int type) const override {
    return type == TestLexer::TYPE_PLUS ? 10 : 0;
  }

  StatusOr<std::unique_ptr<Node>> ParsePrefixToken(
      std::unique_ptr<const Token> token) override {
    if (token->IsType(TestLexer::TYPE_DIGIT)) {
      auto node = std::unique_ptr<Node>(new Node(std::move(token)));

      // Weird behavior only for the test.
      if (node->token().value() == "0") {
        RETURN_IF_ERROR(ConsumeToken(TestLexer::TYPE_DIGIT));
      }

      return std::move(node);
    }
    return UnexpectedToken(*token);
  }

  StatusOr<std::unique_ptr<Node>> ParseInfixToken(
      std::unique_ptr<const Token> token, std::unique_ptr<const Node> left)
      override {
    if (token->IsType(TestLexer::TYPE_PLUS)) {
      auto node = std::unique_ptr<Node>(new Node(std::move(token)));
      node->AddChild(std::move(left));

      ASSIGN_OR_RETURN(auto right, ParseExpression(10));
      node->AddChild(std::move(right));

      return std::move(node);
    }
    return UnexpectedToken(*token);
  }
};

class ParserTest : public testing::Test {
 protected:
  StatusOr<std::unique_ptr<Node>> Parse(const char* input) {
    TestLexer lexer;
    TokenStream stream(&lexer, input);
    TestParser parser(&stream);
    return parser.Parse();
  }

  void ExpectNode(const StatusOr<std::unique_ptr<Node>>& status_or,
                  int type, int num_children) {
    EXPECT_TRUE(status_or.ok());
    const auto& node = status_or.value();
    EXPECT_EQ(type, node->token().type());
    EXPECT_EQ(num_children, node->children().size());
  }

  void ExpectNode(const StatusOr<std::unique_ptr<Node>>& status_or,
                  int type, const std::string& value, int num_children) {
    ExpectNode(status_or, type, num_children);
    EXPECT_EQ(value, status_or.value()->token().value());
  }
};

TEST_F(ParserTest, ExpectToken) {
  Token token(1, "a", 2, 3);
  EXPECT_OK(Parser::ExpectToken(token, 1));
  EXPECT_STATUS(Parser::ExpectToken(token, 2), "Unexpected token: a", 2, 3);
}

TEST_F(ParserTest, UnexpectedToken) {
  Token token(1, "a", 2, 3);
  EXPECT_STATUS(Parser::UnexpectedToken(token), "Unexpected token: a", 2, 3);
}

TEST_F(ParserTest, Empty) {
  EXPECT_STATUS(Parse("").status(), "Unexpected token: (end of input)", 1, 1);
}

TEST_F(ParserTest, UnexpectedCharacter) {
  EXPECT_STATUS(Parse("a").status(), "Unexpected character: a", 1, 1);
}

TEST_F(ParserTest, Prefix) {
  ExpectNode(Parse("1"), TestLexer::TYPE_DIGIT, 0);
}

TEST_F(ParserTest, PrefixError) {
  EXPECT_STATUS(Parse("+").status(), "Unexpected token: +", 1, 1);
}

TEST_F(ParserTest, Infix) {
  ExpectNode(Parse("1+2"), TestLexer::TYPE_PLUS, 2);
}

TEST_F(ParserTest, InfixError) {
  EXPECT_STATUS(Parse("1+").status(), "Unexpected token: (end of input)", 1, 3);
}

TEST_F(ParserTest, ConsumeToken) {
  ExpectNode(Parse("01"), TestLexer::TYPE_DIGIT, "0", 0);
}

TEST_F(ParserTest, ConsumeTokenError) {
  EXPECT_STATUS(Parse("0").status(), "Unexpected token: (end of input)", 1, 2);
}

TEST_F(ParserTest, ParseMultiple) {
  TestLexer lexer;
  TokenStream stream(&lexer, "1 1 1");
  TestParser parser(&stream);

  for (int i = 0; i < 3; ++i) {
    ExpectNode(parser.Parse(), TestLexer::TYPE_DIGIT, "1", 0);
  }

  EXPECT_STATUS(parser.Parse().status(), "Unexpected token: (end of input)", 1, 6);
}
