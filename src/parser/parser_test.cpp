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

  virtual bool GetToken(const std::string& input,
                        int index,
                        int* type,
                        std::string* value,
                        int* count,
                        std::string* error) const {
    const char& c = input[index];
    if (c == '+') {
      *type = TYPE_PLUS;
    } else if (IsDigit(c)) {
      *type = TYPE_DIGIT;
    } else {
      *error = std::string("Unexpected token: %c") + c;
      return false;
    }

    *value = c;
    *count = 1;
    return true;
  }
};

class TestParser : public Parser {
 public:
  TestParser(TokenStream* token_stream) : Parser(token_stream) {
  }

 protected:
  virtual int GetBindingPower(int type) const {
    if (type == TestLexer::TYPE_PLUS)
        return 10;

    return 0;
  }

  virtual bool ParsePrefixToken(std::unique_ptr<const Token> token,
                                std::unique_ptr<const ASTNode>* root) {
    if (token->IsType(TestLexer::TYPE_DIGIT)) {
      root->reset(new ASTNode(std::move(token)));

      // Weird behavior only for the test.
      if (root->get()->token()->value() == "0")
        return ConsumeToken(TestLexer::TYPE_DIGIT);

      return true;
    }

    position_ = token->position();
    error_ = "Unexpected token: " + token->value();
    return false;
  }

  virtual bool ParseInfixToken(std::unique_ptr<const Token> token,
                               std::unique_ptr<const ASTNode> left,
                               std::unique_ptr<const ASTNode>* root) {
    if (token->IsType(TestLexer::TYPE_PLUS)) {
      std::unique_ptr<ASTNode> node(new ASTNode(std::move(token)));
      node->AddChild(std::move(left));

      std::unique_ptr<const ASTNode> right;
      if (!ParseExpression(10, &right))
        return false;
      node->AddChild(std::move(right));

      *root = std::move(node);
      return true;
    }

    position_ = token->position();
    error_ = "Unexpected token: " + token->value();
    return false;
  }
};

class ParserTest : public testing::Test {
 protected:
  void Init(const char* input) {
    stream_.reset(new TokenStream(&lexer_, input));
    parser_.reset(new TestParser(stream_.get()));
  }

  TestLexer lexer_;
  std::unique_ptr<TokenStream> stream_;
  std::unique_ptr<Parser> parser_;
  std::unique_ptr<const ASTNode> root_;
};

TEST_F(ParserTest, Empty) {
  Init("");
  EXPECT_FALSE(parser_->HasInput());
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_EQ(nullptr, root_.get());
  EXPECT_FALSE(parser_->HasInput());
}

TEST_F(ParserTest, BadToken) {
  Init("a");
  EXPECT_TRUE(parser_->HasInput());
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());
  EXPECT_TRUE(parser_->HasInput());
}

TEST_F(ParserTest, Prefix) {
  Init("1");
  EXPECT_TRUE(parser_->HasInput());
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_NE(nullptr, root_.get());
  EXPECT_TRUE(root_->token()->IsType(TestLexer::TYPE_DIGIT));
  EXPECT_EQ(0, root_->children().size());

  EXPECT_FALSE(parser_->HasInput());
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_EQ(nullptr, root_.get());
}

TEST_F(ParserTest, PrefixError) {
  Init("+");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());
}

TEST_F(ParserTest, Infix) {
  Init("1+2");
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_NE(nullptr, root_.get());
  EXPECT_TRUE(root_->token()->IsType(TestLexer::TYPE_PLUS));
  EXPECT_EQ(2, root_->children().size());

  const ASTNode* child = root_->children()[0].get();
  EXPECT_TRUE(child->token()->IsType(TestLexer::TYPE_DIGIT));
  EXPECT_EQ("1", child->token()->value());
  EXPECT_EQ(0, child->children().size());

  child = root_->children()[1].get();
  EXPECT_TRUE(child->token()->IsType(TestLexer::TYPE_DIGIT));
  EXPECT_EQ("2", child->token()->value());
  EXPECT_EQ(0, child->children().size());

  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_EQ(nullptr, root_.get());
}

TEST_F(ParserTest, InfixError) {
  Init("1+");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());
}

TEST_F(ParserTest, ConsumeToken) {
  Init("01");
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_NE(nullptr, root_.get());
  EXPECT_TRUE(root_->token()->IsType(TestLexer::TYPE_DIGIT));
  EXPECT_EQ("0", root_->token()->value());
  EXPECT_EQ(0, root_->children().size());

  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_EQ(nullptr, root_.get());
}

TEST_F(ParserTest, ConsumeTokenError) {
  Init("0");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_EQ(1, parser_->position().line);
  EXPECT_EQ(2, parser_->position().column);
  EXPECT_FALSE(parser_->error().empty());
}

TEST_F(ParserTest, ParseMultiple) {
  Init("1 1 1");

  for (int i = 0; i < 3; ++i) {
    EXPECT_TRUE(parser_->Parse(&root_));
    EXPECT_NE(nullptr, root_.get());
    EXPECT_TRUE(root_->token()->IsType(TestLexer::TYPE_DIGIT));
    EXPECT_EQ(0, root_->children().size());
  }

  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_EQ(nullptr, root_.get());

  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_EQ(nullptr, root_.get());
}
