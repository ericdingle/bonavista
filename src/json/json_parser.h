#ifndef JSON_JSON_PARSER_H_
#define JSON_JSON_PARSER_H_

#include "parser/parser.h"

class JsonParser : public Parser {
 public:
  JsonParser(TokenStream* token_stream);
  JsonParser(const JsonParser&) = delete;
  JsonParser& operator=(const JsonParser&) = delete;
  virtual ~JsonParser();

  virtual bool Parse(std::unique_ptr<const ASTNode>* root);

 protected:
  virtual bool ParsePrefixToken(std::unique_ptr<const Token> token,
                                std::unique_ptr<const ASTNode>* root);

 private:
  bool ParseObject(std::unique_ptr<const Token> token,
                   std::unique_ptr<const ASTNode>* root);
  bool ParseArray(std::unique_ptr<const Token> token,
                  std::unique_ptr<const ASTNode>* root);
};

#endif  // JSON_JSON_PARSER_H_
