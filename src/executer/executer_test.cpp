#include "executer/executer.h"

#include <gtest/gtest.h>

#include "executer/executer_test_fixture.h"
#include "lexer/lexer.h"
#include "parser/node.h"
#include "parser/parser.h"

class TestLexer : public Lexer {
 public:
  enum Type {
    TYPE_DIGIT
  };

  absl::StatusOr<std::unique_ptr<Token>> GetToken(
      std::string_view input, int line, int column) const override {
    RETURN_IF_ERROR(ExpectDigit(input[0], line, column));
    return std::make_unique<Token>(TYPE_DIGIT, input.substr(0, 1), line, column);
  }
};

class TestParser : public Parser {
 public:
  using Parser::Parser;

 protected:
  absl::StatusOr<std::unique_ptr<Node>> ParsePrefixToken(
      std::unique_ptr<const Token> token) override {
    return std::make_unique<Node>(std::move(token));
  }
};

class TestExecuter : public Executer {
 public:
  using Executer::Executer;
  using Executer::ExecuteNodeT;

 protected:
  absl::StatusOr<std::any> ExecuteNode(const Node& node) override {
    int digit = node.token().value()[0] - 0x30;
    if (digit == 9) {
      return Error("No nines!", node.token().line(), node.token().column());
    }

    return std::any(digit);
  }
};

class ExecuterTest
    : public ExecuterTestFixture<TestLexer, TestParser, TestExecuter> {
};

TEST_F(ExecuterTest, Empty) {
  EXPECT_EQ(Execute("").status().message(), "Unexpected token '(end of input)' at 1:1.");
}

TEST_F(ExecuterTest, BadToken) {
  EXPECT_EQ(Execute("a").status().message(), "Unexpected character 'a' at 1:1.");
}

TEST_F(ExecuterTest, Execute) {
  EXPECT_EQ(5, std::any_cast<int>(Execute("5").value()));
}

TEST_F(ExecuterTest, ExecuteError) {
  EXPECT_EQ(Execute("9").status().message(), "No nines! at 1:1.");
}

TEST_F(ExecuterTest, ExecuteAll) {
  EXPECT_OK(ExecuteAll("1 2 3 4 5"));
}

TEST_F(ExecuterTest, ExecuteAllError) {
  EXPECT_EQ(ExecuteAll("1 3 5 7 9").message(), "No nines! at 1:9.");
}

TEST_F(ExecuterTest, ExecuteNodeT) {
  TestExecuter executer(nullptr);

  Node node(std::make_unique<Token>(TestLexer::TYPE_DIGIT, "1", 1, 1));
  EXPECT_EQ(1, executer.ExecuteNodeT<int>(node).value());

  EXPECT_EQ(executer.ExecuteNodeT<double>(node).status().message(),
            "Expected type 'd' at 1:1.");
}
