#include "parser/node.h"

Node::Node(std::unique_ptr<const Token> token) : token_(std::move(token)) {
}

Node::~Node() {
}

const Token& Node::token() const {
  return *token_.get();
}

void Node::AddChild(std::unique_ptr<const Node> node) {
  children_.push_back(std::move(node));
}

const std::vector<std::unique_ptr<const Node>>& Node::children() const {
  return children_;
}
