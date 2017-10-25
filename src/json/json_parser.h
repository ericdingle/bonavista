#ifndef JSON_JSON_PARSER_H_
#define JSON_JSON_PARSER_H_

#include "parser/parser.h"

class JsonParser : public Parser {
 public:
  JsonParser(TokenStream* token_stream);
  JsonParser(const JsonParser&) = delete;
  JsonParser& operator=(const JsonParser&) = delete;
  ~JsonParser() override = default;

  StatusOr<std::unique_ptr<Node>> Parse() override;

 protected:
  StatusOr<std::unique_ptr<Node>> ParsePrefixToken(
      std::unique_ptr<const Token> token) override;

 private:
  StatusOr<std::unique_ptr<Node>> ParseObject(
      std::unique_ptr<const Token> token);
  StatusOr<std::unique_ptr<Node>> ParseArray(
      std::unique_ptr<const Token> token);
};

#endif  // JSON_JSON_PARSER_H_
