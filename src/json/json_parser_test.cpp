#include "json/json_parser.h"

#include "lexer/token_stream.h"
#include "json/json_lexer.h"
#include "parser/ast_node.h"
#include "third_party/googletest/googletest/include/gtest/gtest.h"

class JsonParserTest : public testing::Test {
 protected:
  void CreateParser(const char* input) {
    stream_.reset(new TokenStream(&lexer_, input));
    parser_.reset(new JsonParser(stream_.get()));
  }

  void ExpectStatus(const char* input, const std::string& message) {
    CreateParser(input);
    auto status_or = parser_->Parse();
    EXPECT_TRUE(status_or.error());
    EXPECT_EQ(message, status_or.status().message());
  }

  void ExpectToken(const char* input, int type) {
    CreateParser(input);
    auto status_or = parser_->Parse();
    ASSERT_FALSE(status_or.error()) << status_or.status().ToString();
    EXPECT_EQ(type, status_or.value()->token().type());
  }

  JsonLexer lexer_;
  std::unique_ptr<TokenStream> stream_;
  std::unique_ptr<Parser> parser_;
};

TEST_F(JsonParserTest, ParseEmpty) {
  ExpectStatus("", "Unexpected token: (end of input)");
}

TEST_F(JsonParserTest, ParseUnknown) {
  ExpectStatus("blah", "Unexpected character: b");
}

TEST_F(JsonParserTest, ParsePrimitive) {
  ExpectToken("false", JsonLexer::TYPE_FALSE);
  ExpectToken("null", JsonLexer::TYPE_NULL);
  ExpectToken("1", JsonLexer::TYPE_NUMBER);
  ExpectToken("\"test\"", JsonLexer::TYPE_STRING);
  ExpectToken("true", JsonLexer::TYPE_TRUE);
}

TEST_F(JsonParserTest, ParseObject) {
  ExpectToken("{}", JsonLexer::TYPE_LEFT_BRACE);

  CreateParser("{\"a\": 1, \"b\": false}");
  auto status_or = parser_->Parse();
  EXPECT_FALSE(status_or.error());
  const auto& root = status_or.value();
  EXPECT_EQ(JsonLexer::TYPE_LEFT_BRACE, root->token().type());
  EXPECT_EQ(4, root->children().size());

  const auto& children = root->children();
  EXPECT_EQ(JsonLexer::TYPE_STRING, children[0]->token().type());
  EXPECT_EQ(JsonLexer::TYPE_NUMBER, children[1]->token().type());
  EXPECT_EQ(JsonLexer::TYPE_STRING, children[2]->token().type());
  EXPECT_EQ(JsonLexer::TYPE_FALSE, children[3]->token().type());
}

TEST_F(JsonParserTest, ParseObjectError) {
  ExpectStatus("{, \"a\": 1}", "Unexpected token: ,");
  ExpectStatus("{1: false}", "Unexpected token: 1");
  ExpectStatus("{\"a\", false}", "Unexpected token: ,");
  ExpectStatus("{\"a\": }", "Unexpected token: }");
  ExpectStatus("{\"a\": false", "Unexpected token: (end of input)");
}

TEST_F(JsonParserTest, ParseArray) {
  ExpectToken("[]", JsonLexer::TYPE_LEFT_BRACKET);

  CreateParser("[1, false, \"test\"]");
  auto status_or = parser_->Parse();
  EXPECT_FALSE(status_or.error());
  const auto& root = status_or.value();
  EXPECT_EQ(JsonLexer::TYPE_LEFT_BRACKET, root->token().type());
  EXPECT_EQ(3, root->children().size());

  const auto& children = root->children();
  EXPECT_EQ(JsonLexer::TYPE_NUMBER, children[0]->token().type());
  EXPECT_EQ(JsonLexer::TYPE_FALSE, children[1]->token().type());
  EXPECT_EQ(JsonLexer::TYPE_STRING, children[2]->token().type());
}

TEST_F(JsonParserTest, ParseArrayError) {
  ExpectStatus("[, false]", "Unexpected token: ,");
  ExpectStatus("[1, false, null", "Unexpected token: (end of input)");
}

TEST_F(JsonParserTest, ParseMultipleExpressions) {
  ExpectStatus("1 false", "Unexpected token: false");
  ExpectStatus("1 blah", "Unexpected character: b");
}
