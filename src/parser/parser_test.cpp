#include "parser/parser.h"

#include "lexer/lexer.h"
#include "lexer/token_stream.h"
#include "parser/ast_node.h"
#include "third_party/googletest/googletest/include/gtest/gtest.h"

class TestLexer : public Lexer {
 public:
  enum Type {
    TYPE_DIGIT,
    TYPE_PLUS
  };

  StatusOr<std::unique_ptr<Token>> GetToken(
      const char* input, int line, int column) const override {
    char c = input[0];
    if (c == '+') {
      return std::unique_ptr<Token>(new Token(TYPE_PLUS, "+", line, column));
    } else if (IsDigit(c)) {
      return std::unique_ptr<Token>(new Token(TYPE_DIGIT, std::string(1, c),
                                              line, column));
    }

    return UnexpectedCharacter(c, line, column);
  }
};

class TestParser : public Parser {
 public:
  TestParser(TokenStream* token_stream) : Parser(token_stream) {
  }

 protected:
  int GetBindingPower(int type) const override {
    return type == TestLexer::TYPE_PLUS ? 10 : 0;
  }

  StatusOr<std::unique_ptr<ASTNode>> ParsePrefixToken(
      std::unique_ptr<const Token> token) override {
    if (token->IsType(TestLexer::TYPE_DIGIT)) {
      auto node = std::unique_ptr<ASTNode>(new ASTNode(std::move(token)));

      // Weird behavior only for the test.
      if (node->token().value() == "0") {
        RETURN_IF_ERROR(ConsumeToken(TestLexer::TYPE_DIGIT));
      }

      return std::move(node);
    }

    return UnexpectedToken(*token);
  }

  StatusOr<std::unique_ptr<ASTNode>> ParseInfixToken(
      std::unique_ptr<const Token> token, std::unique_ptr<const ASTNode> left)
      override {
    if (token->IsType(TestLexer::TYPE_PLUS)) {
      auto node = std::unique_ptr<ASTNode>(new ASTNode(std::move(token)));
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
  void Init(const char* input) {
    stream_.reset(new TokenStream(&lexer_, input));
    parser_.reset(new TestParser(stream_.get()));
  }

  void ExpectStatus(const StatusOr<std::unique_ptr<ASTNode>>& status_or,
                    const std::string& message) {
    EXPECT_TRUE(status_or.error());
    EXPECT_EQ(message, status_or.status().message());
  }

  void ExpectNode(const StatusOr<std::unique_ptr<ASTNode>>& status_or,
                  int type, int num_children) {
    EXPECT_FALSE(status_or.error());
    const auto& node = status_or.value();
    EXPECT_EQ(type, node->token().type());
    EXPECT_EQ(num_children, node->children().size());
  }

  void ExpectNode(const StatusOr<std::unique_ptr<ASTNode>>& status_or,
                  int type, const std::string& value, int num_children) {
    ExpectNode(status_or, type, num_children);
    EXPECT_EQ(value, status_or.value()->token().value());
  }

  TestLexer lexer_;
  std::unique_ptr<TokenStream> stream_;
  std::unique_ptr<Parser> parser_;
};

TEST_F(ParserTest, ExpectToken) {
  Token token(1, "a", 2, 3);
  Status s1 = Parser::ExpectToken(token, 1);
  EXPECT_FALSE(s1.error());
  Status s2 = Parser::ExpectToken(token, 2);
  EXPECT_TRUE(s2.error());
  EXPECT_EQ("Unexpected token: a", s2.message());
}

TEST_F(ParserTest, UnexpectedToken) {
  Token token(1, "a", 2, 3);
  Status s = Parser::UnexpectedToken(token);
  EXPECT_TRUE(s.error());
  EXPECT_EQ("Unexpected token: a", s.message());
  EXPECT_EQ(2, s.line());
  EXPECT_EQ(3, s.column());
}

TEST_F(ParserTest, Empty) {
  Init("");
  EXPECT_FALSE(parser_->HasInput());
  ExpectStatus(parser_->Parse(), "Unexpected token: (end of input)");
}

TEST_F(ParserTest, NotEmpty) {
  Init("a");
  EXPECT_TRUE(parser_->HasInput());
}

TEST_F(ParserTest, UnexpectedCharacter) {
  Init("a");
  ExpectStatus(parser_->Parse(), "Unexpected character: a");
}

TEST_F(ParserTest, Prefix) {
  Init("1");
  ExpectNode(parser_->Parse(), TestLexer::TYPE_DIGIT, 0);
}

TEST_F(ParserTest, PrefixError) {
  Init("+");
  ExpectStatus(parser_->Parse(), "Unexpected token: +");
}

TEST_F(ParserTest, Infix) {
  Init("1+2");
  ExpectNode(parser_->Parse(), TestLexer::TYPE_PLUS, 2);
}

TEST_F(ParserTest, InfixError) {
  Init("1+");
  ExpectStatus(parser_->Parse(), "Unexpected token: (end of input)");
}

TEST_F(ParserTest, ConsumeToken) {
  Init("01");
  ExpectNode(parser_->Parse(), TestLexer::TYPE_DIGIT, "0", 0);
}

TEST_F(ParserTest, ConsumeTokenError) {
  Init("0");
  ExpectStatus(parser_->Parse(), "Unexpected token: (end of input)");
}

TEST_F(ParserTest, ParseMultiple) {
  Init("1 1 1");

  for (int i = 0; i < 3; ++i) {
    ExpectNode(parser_->Parse(), TestLexer::TYPE_DIGIT, "1", 0);
  }

  ExpectStatus(parser_->Parse(), "Unexpected token: (end of input)");
}
