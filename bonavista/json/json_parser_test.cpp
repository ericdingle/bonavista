#include "bonavista/json/json_parser.h"

#include "bonavista/json/json_lexer.h"
#include "bonavista/lexer/token_stream.h"
#include "bonavista/lexer/token_test_macros.h"
#include "bonavista/parser/node.h"
#include "bonavista/parser/parser_test_fixture.h"
#include "bonavista/util/status_macros.h"

class JsonParserTest : public ParserTestFixture<JsonLexer, JsonParser> {};

TEST_F(JsonParserTest, ParseEmpty) {
  EXPECT_EQ(Parse("").status().message(),
            "Unexpected token '(end of input)' at 1:1.");
}

TEST_F(JsonParserTest, ParseUnknown) {
  EXPECT_EQ(Parse("blah").status().message(),
            "Unexpected character 'b' at 1:1.");
}

TEST_F(JsonParserTest, ParsePrimitive) {
  EXPECT_TOKEN(Parse("false").value()->token(), JsonLexer::TYPE_KEYWORD,
               "false", 1, 1);
  EXPECT_TOKEN(Parse("1").value()->token(), JsonLexer::TYPE_NUMBER, "1", 1, 1);
  EXPECT_TOKEN(Parse("\"test\"").value()->token(), JsonLexer::TYPE_STRING,
               "test", 1, 1);
}

TEST_F(JsonParserTest, ParseObject) {
  EXPECT_TOKEN(Parse("{}").value()->token(), JsonLexer::TYPE_LEFT_BRACE, "{", 1,
               1);

  std::unique_ptr<Node> node = Parse("{\"a\": 1, \"b\": false}").value();
  EXPECT_TOKEN(node->token(), JsonLexer::TYPE_LEFT_BRACE, "{", 1, 1);
  EXPECT_EQ(4U, node->children().size());

  const auto& children = node->children();
  EXPECT_TOKEN(children[0]->token(), JsonLexer::TYPE_STRING, "a", 1, 2);
  EXPECT_TOKEN(children[1]->token(), JsonLexer::TYPE_NUMBER, "1", 1, 7);
  EXPECT_TOKEN(children[2]->token(), JsonLexer::TYPE_STRING, "b", 1, 10);
  EXPECT_TOKEN(children[3]->token(), JsonLexer::TYPE_KEYWORD, "false", 1, 15);
}

TEST_F(JsonParserTest, ParseObjectError) {
  EXPECT_EQ(Parse("{, \"a\": 1}").status().message(),
            "Unexpected token ',' at 1:2.");
  EXPECT_EQ(Parse("{1: false}").status().message(),
            "Unexpected token '1' at 1:2.");
  EXPECT_EQ(Parse("{\"a\", false}").status().message(),
            "Unexpected token ',' at 1:5.");
  EXPECT_EQ(Parse("{\"a\": }").status().message(),
            "Unexpected token '}' at 1:7.");
  EXPECT_EQ(Parse("{\"a\": false").status().message(),
            "Unexpected token '(end of input)' at 1:12.");
}

TEST_F(JsonParserTest, ParseArray) {
  EXPECT_TOKEN(Parse("[]").value()->token(), JsonLexer::TYPE_LEFT_BRACKET, "[",
               1, 1);

  std::unique_ptr<Node> node = Parse("[1, false, \"test\"]").value();
  EXPECT_TOKEN(node->token(), JsonLexer::TYPE_LEFT_BRACKET, "[", 1, 1);
  EXPECT_EQ(3U, node->children().size());

  const auto& children = node->children();
  EXPECT_TOKEN(children[0]->token(), JsonLexer::TYPE_NUMBER, "1", 1, 2);
  EXPECT_TOKEN(children[1]->token(), JsonLexer::TYPE_KEYWORD, "false", 1, 5);
  EXPECT_TOKEN(children[2]->token(), JsonLexer::TYPE_STRING, "test", 1, 12);
}

TEST_F(JsonParserTest, ParseArrayError) {
  EXPECT_EQ(Parse("[, false]").status().message(),
            "Unexpected token ',' at 1:2.");
  EXPECT_EQ(Parse("[1, false, null").status().message(),
            "Unexpected token '(end of input)' at 1:16.");
}

TEST_F(JsonParserTest, ParseMultipleExpressions) {
  EXPECT_EQ(Parse("1 false").status().message(),
            "Unexpected token 'false' at 1:3.");
  EXPECT_EQ(Parse("1 blah").status().message(),
            "Unexpected character 'b' at 1:3.");
}
