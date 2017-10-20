#include "lexer/token.h"

Token::Token(int type, const std::string& value, int line, int column)
    : type_(type), value_(value), line_(line), column_(column) {
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

int Token::line() const {
  return line_;
}

int Token::column() const {
  return column_;
}
