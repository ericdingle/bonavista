#include "lexer/token.h"

Token::Token(int type, const std::string& value, const Position& position)
    : type_(type), value_(value), position_(position) {
}

Token::~Token() {
}

bool Token::IsType(int type) const {
  return type_ == type;
}

int Token::type() const {
  return type_;
}

const std::string& Token::value() const {
  return value_;
}

const Token::Position& Token::position() const {
  return position_;
}
