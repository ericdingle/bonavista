#include "bonavista/executer/executer.h"

#include <assert.h>

#include "bonavista/parser/parser.h"

Executer::Executer(Parser* parser) : parser_(parser) {
}

absl::StatusOr<std::any> Executer::Execute() {
  ASSIGN_OR_RETURN(std::unique_ptr<Node> node, parser_->Parse());
  assert(node);
  return ExecuteNode(*node);
}

absl::Status Executer::ExecuteAll() {
  while (HasInput()) {
    ASSIGN_OR_RETURN(const std::any any, Execute());
  }
  return absl::OkStatus();
}

bool Executer::HasInput() const {
  return parser_->HasInput();
}
