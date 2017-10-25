#ifndef PARSER_NODE_H_
#define PARSER_NODE_H_

#include <memory>
#include <vector>
#include "lexer/token.h"

class Node {
 public:
  Node(std::unique_ptr<const Token> token);
  Node(const Node&) = delete;
  Node& operator=(const Node&) = delete;
  ~Node();

  const Token& token() const;

  void AddChild(std::unique_ptr<const Node> node);
  const std::vector<std::unique_ptr<const Node>>& children() const;

 private:
  const std::unique_ptr<const Token> token_;
  std::vector<std::unique_ptr<const Node>> children_;
};

#endif  // PARSER_NODE_H
