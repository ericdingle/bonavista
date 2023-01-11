#include "parser/node.h"

Node::Node(std::unique_ptr<const Token> token) : token_(std::move(token)) {}

Node::~Node() {}

void Node::AddChild(std::unique_ptr<const Node> node) {
  children_.push_back(std::move(node));
}
