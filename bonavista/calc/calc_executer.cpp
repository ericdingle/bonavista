#include "bonavista/calc/calc_executer.h"

#include "bonavista/calc/calc_lexer.h"
#include "bonavista/util/error.h"

absl::StatusOr<std::any> CalcExecuter::ExecuteNode(const Node& node) {
  if (node.token().IsType(CalcLexer::TYPE_ASTERISK) ||
      node.token().IsType(CalcLexer::TYPE_MINUS) ||
      node.token().IsType(CalcLexer::TYPE_PLUS) ||
      node.token().IsType(CalcLexer::TYPE_SLASH)) {
    ASSIGN_OR_RETURN(double left, ExecuteNodeT<double>(*node.children()[0]));
    ASSIGN_OR_RETURN(double right, ExecuteNodeT<double>(*node.children()[1]));

    double result = 0;
    if (node.token().IsType(CalcLexer::TYPE_ASTERISK)) {
      result = left * right;
    } else if (node.token().IsType(CalcLexer::TYPE_MINUS)) {
      result = left - right;
    } else if (node.token().IsType(CalcLexer::TYPE_PLUS)) {
      result = left + right;
    } else {
      result = left / right;
    }

    return std::any(result);
  }

  if (node.token().IsType(CalcLexer::TYPE_NUMBER)) {
    double value = strtod(node.token().value().data(), nullptr);
    return std::any(value);
  }

  return Error("WTF!", 1, 1);
}
