#include "lexer/token_stream.h"
#include "lexer/token_test_macros.h"
#include "json/json_lexer.h"
#include "json/json_parser.h"
#include "parser/node.h"
#include "parser/parser_test_fixture.h"
#include "util/status_test_macros.h"

class JsonParserTest : public ParserTestFixture<JsonLexer, JsonParser> {
};

TEST_F(JsonParserTest, ParseEmpty) {
  EXPECT_STATUS(Parse("").status(), "Unexpected token: (end of input)", 1, 1);
}

TEST_F(JsonParserTest, ParseUnknown) {
  EXPECT_STATUS(Parse("blah").status(), "Unexpected character: b", 1, 1);
}

TEST_F(JsonParserTest, ParsePrimitive) {
  EXPECT_TOKEN(Parse("false").value()->token(), JsonLexer::TYPE_KEYWORD, "false", 1, 1);
  EXPECT_TOKEN(Parse("1").value()->token(), JsonLexer::TYPE_NUMBER, "1", 1, 1);
  EXPECT_TOKEN(Parse("\"test\"").value()->token(), JsonLexer::TYPE_STRING, "test", 1, 1);
}

TEST_F(JsonParserTest, ParseObject) {
  EXPECT_TOKEN(Parse("{}").value()->token(), JsonLexer::TYPE_LEFT_BRACE, "{", 1, 1);

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
  EXPECT_STATUS(Parse("{, \"a\": 1}").status(), "Unexpected token: ,", 1, 2);
  EXPECT_STATUS(Parse("{1: false}").status(), "Unexpected token: 1", 1, 2);
  EXPECT_STATUS(Parse("{\"a\", false}").status(), "Unexpected token: ,", 1, 5);
  EXPECT_STATUS(Parse("{\"a\": }").status(), "Unexpected token: }", 1, 7);
  EXPECT_STATUS(Parse("{\"a\": false").status(),
                "Unexpected token: (end of input)", 1, 12);
}

TEST_F(JsonParserTest, ParseArray) {
  EXPECT_TOKEN(Parse("[]").value()->token(), JsonLexer::TYPE_LEFT_BRACKET, "[", 1, 1);

  std::unique_ptr<Node> node = Parse("[1, false, \"test\"]").value();
  EXPECT_TOKEN(node->token(), JsonLexer::TYPE_LEFT_BRACKET, "[", 1, 1);
  EXPECT_EQ(3U, node->children().size());

  const auto& children = node->children();
  EXPECT_TOKEN(children[0]->token(), JsonLexer::TYPE_NUMBER, "1", 1, 2);
  EXPECT_TOKEN(children[1]->token(), JsonLexer::TYPE_KEYWORD, "false", 1, 5);
  EXPECT_TOKEN(children[2]->token(), JsonLexer::TYPE_STRING, "test", 1, 12);
}

TEST_F(JsonParserTest, ParseArrayError) {
  EXPECT_STATUS(Parse("[, false]").status(), "Unexpected token: ,", 1, 2);
  EXPECT_STATUS(Parse("[1, false, null").status(),
                "Unexpected token: (end of input)", 1, 16);
}

TEST_F(JsonParserTest, ParseMultipleExpressions) {
  EXPECT_STATUS(Parse("1 false").status(), "Unexpected token: false", 1, 3);
  EXPECT_STATUS(Parse("1 blah").status(), "Unexpected character: b", 1, 3);
}
