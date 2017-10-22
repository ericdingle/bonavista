#ifndef PARSER_PARSER_H_
#define PARSER_PARSER_H_

#include <memory>
#include <string>
#include "lexer/token.h"
#include "lexer/token_stream.h"
#include "parser/ast_node.h"
#include "util/status_or.h"

class Parser {
 public:
  Parser(TokenStream* token_stream);
  Parser(const Parser&) = delete;
  Parser& operator=(const Parser&) = delete;
  virtual ~Parser();

  virtual StatusOr<std::unique_ptr<ASTNode>> Parse();

  bool HasInput() const;

 protected:
  static Status UnexpectedToken(const Token& token);

  StatusOr<std::unique_ptr<Token>> GetNextToken();
  StatusOr<std::unique_ptr<ASTNode>> ParseExpression(int binding_power);
  Status ConsumeToken(int type);

  virtual int GetBindingPower(int type) const;
  virtual StatusOr<std::unique_ptr<ASTNode>> ParsePrefixToken(
      std::unique_ptr<const Token> token) = 0;
  virtual StatusOr<std::unique_ptr<ASTNode>> ParseInfixToken(
      std::unique_ptr<const Token> token, std::unique_ptr<const ASTNode> left);

  std::unique_ptr<Token> look_ahead_token_;

 private:
  TokenStream* const token_stream_;
};

#endif  // PARSER_PARSER_H_
