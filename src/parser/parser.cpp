#include "parser/parser.h"

#include "lexer/lexer.h"

Parser::Parser(TokenStream* token_stream) : token_stream_(token_stream) {
}

Parser::~Parser() {
}

bool Parser::Parse(std::unique_ptr<const ASTNode>* root) {
  if (!Init())
    return false;

  // Check for end of input.
  if (look_ahead_token_->IsType(Lexer::TYPE_END_OF_INPUT)) {
    root->release();
    return true;
  }

  // Parse expression.
  return ParseExpression(0, root);
}

bool Parser::HasInput() const {
  return look_ahead_token_.get() ?
      !look_ahead_token_->IsType(Lexer::TYPE_END_OF_INPUT) :
      token_stream_->HasInput();
}

const Token::Position& Parser::position() const {
  return position_;
}

const std::string& Parser::error() const {
  return error_;
}

bool Parser::Init() {
  if (look_ahead_token_.get())
    return true;

  // Advance the look ahead token to the first token.
  std::unique_ptr<const Token> token;
  return GetNextToken(&token);
}

bool Parser::GetNextToken(std::unique_ptr<const Token>* token) {
  *token = std::move(look_ahead_token_);

  if (!token_stream_->GetNextToken(&look_ahead_token_)) {
    position_ = token_stream_->position();
    error_ = token_stream_->error();
    return false;
  }

  return true;
}

bool Parser::ParseExpression(int binding_power,
                             std::unique_ptr<const ASTNode>* root) {
  std::unique_ptr<const Token> token;
  if (!GetNextToken(&token))
    return false;

  std::unique_ptr<const ASTNode> left;
  if (!ParsePrefixToken(std::move(token), &left))
    return false;

  while (binding_power < GetBindingPower(look_ahead_token_->type())) {
    if (!GetNextToken(&token))
      return false;

    if (!ParseInfixToken(std::move(token), std::move(left), &left))
      return false;
  }

  *root = std::move(left);
  return true;
}

bool Parser::ConsumeToken(int type) {
  std::unique_ptr<const Token> token;
  if (!GetNextToken(&token))
    return false;

  if (!token->IsType(type)) {
    position_ = token->position();
    error_ = "Unexpected token: " + token->value();
    return false;
  }

  return true;
}

int Parser::GetBindingPower(int type) const {
  return 0;
}

bool Parser::ParseInfixToken(std::unique_ptr<const Token> token,
                             std::unique_ptr<const ASTNode> left,
                             std::unique_ptr<const ASTNode>* root) {
  position_ = token->position();
  error_ = "Unexpected token: " + token->value();
  return false;
}
