#ifndef PARSER_PARSER_H_
#define PARSER_PARSER_H_

#include <memory>
#include <string>
#include "lexer/token.h"
#include "lexer/token_stream.h"
#include "parser/ast_node.h"

class Parser {
 public:
  Parser(TokenStream* token_stream);
  Parser(const Parser&) = delete;
  Parser& operator=(const Parser&) = delete;
  virtual ~Parser();

  virtual bool Parse(std::unique_ptr<const ASTNode>* root);

  bool HasInput() const;

  int line() const;
  int column() const;
  const std::string& error() const;

 private:
  bool Init();

 protected:
  bool GetNextToken(std::unique_ptr<const Token>* token);
  bool ParseExpression(int binding_power, std::unique_ptr<const ASTNode>* root);
  bool ConsumeToken(int type);

  virtual int GetBindingPower(int type) const;
  virtual bool ParsePrefixToken(std::unique_ptr<const Token> token,
                                std::unique_ptr<const ASTNode>* root) = 0;
  virtual bool ParseInfixToken(std::unique_ptr<const Token> token,
                               std::unique_ptr<const ASTNode> left,
                               std::unique_ptr<const ASTNode>* root);

  std::unique_ptr<const Token> look_ahead_token_;

  int line_;
  int column_;
  std::string error_;

 private:
  TokenStream* token_stream_;
};

#endif  // PARSER_PARSER_H_
