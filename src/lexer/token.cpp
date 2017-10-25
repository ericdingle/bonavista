#include "lexer/token.h"

Token::Token(int type, char c, int line, int column)
    : Token(type, std::string(1, c), line, column) {
}

Token::Token(int type, const std::string& value, int line, int column, int length)
    : type_(type), value_(value), line_(line), column_(column), length_(length) {
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

int Token::length() const {
  return length_ ? length_ : value_.length();
}
