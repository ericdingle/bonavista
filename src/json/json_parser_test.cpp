#include "lexer/token_stream.h"
#include "json/json_lexer.h"
#include "json/json_parser.h"
#include "parser/node.h"
#include "util/status_test_macros.h"
#include "third_party/googletest/googletest/include/gtest/gtest.h"

class JsonParserTest : public testing::Test {
 protected:
  StatusOr<std::unique_ptr<Node>> Parse(const char* input) {
    JsonLexer lexer;
    TokenStream stream(&lexer, input);
    JsonParser parser(&stream);
    return parser.Parse();
  }

  void ExpectToken(const char* input, int type) {
    auto status_or = Parse(input);
    ASSERT_TRUE(status_or.ok()) << status_or.status().ToString();
    EXPECT_EQ(type, status_or.value()->token().type());
  }
};

TEST_F(JsonParserTest, ParseEmpty) {
  EXPECT_STATUS(Parse("").status(), "Unexpected token: (end of input)", 1, 1);
}

TEST_F(JsonParserTest, ParseUnknown) {
  EXPECT_STATUS(Parse("blah").status(), "Unexpected character: b", 1, 1);
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

  auto status_or = Parse("{\"a\": 1, \"b\": false}");
  EXPECT_TRUE(status_or.ok());
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
  EXPECT_STATUS(Parse("{, \"a\": 1}").status(), "Unexpected token: ,", 1, 2);
  EXPECT_STATUS(Parse("{1: false}").status(), "Unexpected token: 1", 1, 2);
  EXPECT_STATUS(Parse("{\"a\", false}").status(), "Unexpected token: ,", 1, 5);
  EXPECT_STATUS(Parse("{\"a\": }").status(), "Unexpected token: }", 1, 7);
  EXPECT_STATUS(Parse("{\"a\": false").status(),
                "Unexpected token: (end of input)", 1, 12);
}

TEST_F(JsonParserTest, ParseArray) {
  ExpectToken("[]", JsonLexer::TYPE_LEFT_BRACKET);

  auto status_or = Parse("[1, false, \"test\"]");
  EXPECT_TRUE(status_or.ok());
  const auto& root = status_or.value();
  EXPECT_EQ(JsonLexer::TYPE_LEFT_BRACKET, root->token().type());
  EXPECT_EQ(3, root->children().size());

  const auto& children = root->children();
  EXPECT_EQ(JsonLexer::TYPE_NUMBER, children[0]->token().type());
  EXPECT_EQ(JsonLexer::TYPE_FALSE, children[1]->token().type());
  EXPECT_EQ(JsonLexer::TYPE_STRING, children[2]->token().type());
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
