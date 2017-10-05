#ifndef PARSER_AST_NODE_H_
#define PARSER_AST_NODE_H_

#include <memory>
#include <vector>
#include "lexer/token.h"

class ASTNode {
 public:
  ASTNode(std::unique_ptr<const Token> token);
  ASTNode(const ASTNode&) = delete;
  ASTNode& operator=(const ASTNode&) = delete;
  ~ASTNode();

  const Token* token() const;

  void AddChild(std::unique_ptr<const ASTNode> node);
  const std::vector<std::unique_ptr<const ASTNode>>& children() const;

 private:
  const std::unique_ptr<const Token> token_;
  std::vector<std::unique_ptr<const ASTNode>> children_;
};

#endif  // PARSER_AST_NODE_H
