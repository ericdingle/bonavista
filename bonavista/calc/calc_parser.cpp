#include "calc/calc_parser.h"

#include "calc/calc_lexer.h"
#include "util/status_macros.h"

absl::StatusOr<std::unique_ptr<Node>> CalcParser::Parse() {
  ASSIGN_OR_RETURN(auto root, Parser::Parse());

  if (HasInput())
    return UnexpectedToken(*look_ahead_token_);

  return std::move(root);
}

int CalcParser::GetBindingPower(int type) const {
  switch (type) {
    case CalcLexer::TYPE_ASTERISK:
    case CalcLexer::TYPE_SLASH:
      return 20;
    case CalcLexer::TYPE_MINUS:
    case CalcLexer::TYPE_PLUS:
      return 10;
    default:
      return 0;
  }
}

absl::StatusOr<std::unique_ptr<Node>> CalcParser::ParsePrefixToken(
    std::unique_ptr<const Token> token) {
  if (token->IsType(CalcLexer::TYPE_LEFT_PARENTHESIS)) {
    ASSIGN_OR_RETURN(auto node, ParseExpression(0));
    RETURN_IF_ERROR(ConsumeToken(CalcLexer::TYPE_RIGHT_PARENTHESIS));
    return std::move(node);
  }

  if (token->IsType(CalcLexer::TYPE_NUMBER)) {
    return std::make_unique<Node>(std::move(token));
  }

  return UnexpectedToken(*token);
}

absl::StatusOr<std::unique_ptr<Node>> CalcParser::ParseInfixToken(
    std::unique_ptr<const Token> token, std::unique_ptr<const Node> left) {
  if (token->IsType(CalcLexer::TYPE_ASTERISK) ||
      token->IsType(CalcLexer::TYPE_MINUS) ||
      token->IsType(CalcLexer::TYPE_PLUS) ||
      token->IsType(CalcLexer::TYPE_SLASH)) {
    auto node = std::make_unique<Node>(std::move(token));
    node->AddChild(std::move(left));

    ASSIGN_OR_RETURN(auto right,
                     ParseExpression(GetBindingPower(node->token().type())));
    node->AddChild(std::move(right));

    return std::move(node);
  }

  return UnexpectedToken(*token);
}
