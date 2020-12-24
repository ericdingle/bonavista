#ifndef JSON_JSON_PARSER_H_
#define JSON_JSON_PARSER_H_

#include "parser/parser.h"

class JsonParser : public Parser {
 public:
  JsonParser(TokenStream* token_stream);
  JsonParser(const JsonParser&) = delete;
  JsonParser& operator=(const JsonParser&) = delete;
  ~JsonParser() override = default;

  absl::StatusOr<std::unique_ptr<Node>> Parse() override;

 protected:
  absl::StatusOr<std::unique_ptr<Node>> ParsePrefixToken(
      std::unique_ptr<const Token> token) override;

 private:
  absl::StatusOr<std::unique_ptr<Node>> ParseObject(
      std::unique_ptr<const Token> token);
  absl::StatusOr<std::unique_ptr<Node>> ParseArray(
      std::unique_ptr<const Token> token);
};

#endif  // JSON_JSON_PARSER_H_
