#include "json/json_parser.h"

#include "lexer/token_stream.h"
#include "json/json_lexer.h"
#include "parser/ast_node.h"
#include "third_party/googletest/googletest/include/gtest/gtest.h"

class JsonParserTest : public testing::Test {
 protected:
  void Init(const char* input) {
    stream_.reset(new TokenStream(&lexer_, input));
    parser_.reset(new JsonParser(stream_.get()));
  }

  JsonLexer lexer_;
  std::unique_ptr<TokenStream> stream_;
  std::unique_ptr<Parser> parser_;
  std::unique_ptr<const ASTNode> root_;
};

TEST_F(JsonParserTest, ParseEmpty) {
  Init("");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());
}

TEST_F(JsonParserTest, ParseMultiple) {
  Init("1 2");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());
}

TEST_F(JsonParserTest, ParseUnknown) {
  Init("blah");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());
}

TEST_F(JsonParserTest, ParsePrimitive) {
  Init("false");
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_TRUE(root_->token()->IsType(JsonLexer::TYPE_FALSE));

  Init("null");
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_TRUE(root_->token()->IsType(JsonLexer::TYPE_NULL));

  Init("1");
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_TRUE(root_->token()->IsType(JsonLexer::TYPE_NUMBER));

  Init("\"test\"");
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_TRUE(root_->token()->IsType(JsonLexer::TYPE_STRING));

  Init("true");
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_TRUE(root_->token()->IsType(JsonLexer::TYPE_TRUE));
}

TEST_F(JsonParserTest, ParseObject) {
  Init("{}");
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_TRUE(root_->token()->IsType(JsonLexer::TYPE_LEFT_BRACE));
  EXPECT_EQ(0, root_->children().size());

  Init("{\"a\": 1, \"b\": false}");
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_TRUE(root_->token()->IsType(JsonLexer::TYPE_LEFT_BRACE));
  EXPECT_EQ(4, root_->children().size());

  EXPECT_TRUE(root_->children()[0]->token()->IsType(JsonLexer::TYPE_STRING));
  EXPECT_TRUE(root_->children()[1]->token()->IsType(JsonLexer::TYPE_NUMBER));
  EXPECT_TRUE(root_->children()[2]->token()->IsType(JsonLexer::TYPE_STRING));
  EXPECT_TRUE(root_->children()[3]->token()->IsType(JsonLexer::TYPE_FALSE));
}

TEST_F(JsonParserTest, ParseObjectError) {
  Init("{, \"a\": 1}");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());

  Init("{1: false}");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());

  Init("{\"a\", false}");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());

  Init("{\"a\": }");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());

  Init("{\"a\": false");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());
}

TEST_F(JsonParserTest, ParseArray) {
  Init("[]");
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_TRUE(root_->token()->IsType(JsonLexer::TYPE_LEFT_BRACKET));
  EXPECT_EQ(0, root_->children().size());

  Init("[1, false, \"test\"]");
  EXPECT_TRUE(parser_->Parse(&root_));
  EXPECT_TRUE(root_->token()->IsType(JsonLexer::TYPE_LEFT_BRACKET));
  EXPECT_EQ(3, root_->children().size());

  EXPECT_TRUE(root_->children()[0]->token()->IsType(JsonLexer::TYPE_NUMBER));
  EXPECT_TRUE(root_->children()[1]->token()->IsType(JsonLexer::TYPE_FALSE));
  EXPECT_TRUE(root_->children()[2]->token()->IsType(JsonLexer::TYPE_STRING));
}

TEST_F(JsonParserTest, ParseArrayError) {
  Init("[, false");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());

  Init("[1, false, null");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());
}

TEST_F(JsonParserTest, ParseMultipleExpressions) {
  Init("1 false");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());

  Init("1 blah");
  EXPECT_FALSE(parser_->Parse(&root_));
  EXPECT_FALSE(parser_->error().empty());
}
