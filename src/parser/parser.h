#ifndef PARSER_PARSER_H_
#define PARSER_PARSER_H_

#include <memory>
#include <string>

#include "lexer/token.h"
#include "lexer/token_stream.h"
#include "parser/node.h"
#include "third_party/absl/absl/status/statusor.h"
#include "third_party/googletest/googletest/include/gtest/gtest_prod.h"

class Parser {
 public:
  Parser(TokenStream* token_stream);
  Parser(const Parser&) = delete;
  Parser& operator=(const Parser&) = delete;
  virtual ~Parser() = default;

  virtual absl::StatusOr<std::unique_ptr<Node>> Parse();

  bool HasInput() const;

 protected:
  static absl::Status ExpectToken(const Token& token, int type);
  static absl::Status UnexpectedToken(const Token& token);

  absl::StatusOr<std::unique_ptr<Token>> GetNextToken();
  absl::StatusOr<std::unique_ptr<Node>> ParseExpression(int binding_power);
  absl::Status ConsumeToken(int type);

  virtual int GetBindingPower(int type) const;
  virtual absl::StatusOr<std::unique_ptr<Node>> ParsePrefixToken(
      std::unique_ptr<const Token> token) = 0;
  virtual absl::StatusOr<std::unique_ptr<Node>> ParseInfixToken(
      std::unique_ptr<const Token> token, std::unique_ptr<const Node> left);

  std::unique_ptr<Token> look_ahead_token_;

 private:
  TokenStream* const token_stream_;

  FRIEND_TEST(ParserTest, ExpectToken);
  FRIEND_TEST(ParserTest, UnexpectedToken);
};

#endif  // PARSER_PARSER_H_
