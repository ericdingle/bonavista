#include "parser/parser.h"

#include "lexer/lexer.h"
#include "util/status_macros.h"
#include "third_party/absl/absl/strings/str_cat.h"

Parser::Parser(TokenStream* token_stream) : token_stream_(token_stream) {
}

absl::StatusOr<std::unique_ptr<Node>> Parser::Parse() {
  if (!look_ahead_token_) {
    ASSIGN_OR_RETURN(auto token, GetNextToken());
  }

  if (look_ahead_token_->IsType(Lexer::TYPE_END_OF_INPUT)) {
    return UnexpectedToken(*look_ahead_token_);
  }

  return ParseExpression(0);
}

bool Parser::HasInput() const {
  return look_ahead_token_ ?
      !look_ahead_token_->IsType(Lexer::TYPE_END_OF_INPUT) :
      token_stream_->HasInput();
}

absl::Status Parser::ExpectToken(const Token& token, int type) {
  return token.IsType(type) ? absl::OkStatus() : UnexpectedToken(token);
}

absl::Status Parser::UnexpectedToken(const Token& token) {
  return absl::InvalidArgumentError(absl::StrCat(
      "Unexpected token '", token.value(), "' at ", token.line(), ":", token.column(), "."));
}

absl::StatusOr<std::unique_ptr<Token>> Parser::GetNextToken() {
  auto token = std::move(look_ahead_token_);
  ASSIGN_OR_RETURN(look_ahead_token_, token_stream_->GetNextToken());
  return std::move(token);
}

absl::StatusOr<std::unique_ptr<Node>> Parser::ParseExpression(int binding_power) {
  ASSIGN_OR_RETURN(auto token, GetNextToken());
  ASSIGN_OR_RETURN(auto left, ParsePrefixToken(std::move(token)));

  while (binding_power < GetBindingPower(look_ahead_token_->type())) {
    ASSIGN_OR_RETURN(token, GetNextToken());
    ASSIGN_OR_RETURN(left, ParseInfixToken(std::move(token), std::move(left)));
  }

  return std::move(left);
}

absl::Status Parser::ConsumeToken(int type) {
  ASSIGN_OR_RETURN(auto token, GetNextToken());

  if (!token->IsType(type)) {
    return UnexpectedToken(*token);
  }

  return absl::OkStatus();
}

int Parser::GetBindingPower(int type) const {
  return 0;
}

absl::StatusOr<std::unique_ptr<Node>> Parser::ParseInfixToken(
    std::unique_ptr<const Token> token, std::unique_ptr<const Node> left) {
  return UnexpectedToken(*token);
}
