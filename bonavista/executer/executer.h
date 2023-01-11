#ifndef EXECUTER_EXECUTER_H_
#define EXECUTER_EXECUTER_H_

#include <any>

#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/strings/str_cat.h"
#include "bonavista/parser/node.h"
#include "bonavista/parser/parser.h"
#include "bonavista/util/error.h"
#include "bonavista/util/status_macros.h"

class Executer {
 public:
  Executer(Parser* parser);
  Executer(const Executer&) = delete;
  Executer& operator=(const Executer&) = delete;
  virtual ~Executer() = default;

  absl::StatusOr<std::any> Execute();
  absl::Status ExecuteAll();

  bool HasInput() const;

 protected:
  virtual absl::StatusOr<std::any> ExecuteNode(const Node& node) = 0;
  template <typename T>
  absl::StatusOr<T> ExecuteNodeT(const Node& node);

 private:
  Parser* parser_;
};

template <typename T>
absl::StatusOr<T> Executer::ExecuteNodeT(const Node& node) {
  ASSIGN_OR_RETURN(const std::any any, ExecuteNode(node));

  if (any.type() != typeid(T)) {
    return Error(absl::StrCat("Expected type '", typeid(T).name(), "'"),
                 node.token().line(), node.token().column());
  }

  return std::any_cast<T>(any);
}

#endif  // EXECUTER_EXECUTER_H_
