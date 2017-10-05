#include "parser/ast_node.h"

ASTNode::ASTNode(std::unique_ptr<const Token> token) : token_(std::move(token)) {
}

ASTNode::~ASTNode() {
}

const Token* ASTNode::token() const {
  return token_.get();
}

void ASTNode::AddChild(std::unique_ptr<const ASTNode> node) {
  children_.push_back(std::move(node));
}

const std::vector<std::unique_ptr<const ASTNode>>& ASTNode::children() const {
  return children_;
}
