#include "lexer/lexer.h"

const int Lexer::TYPE_END_OF_INPUT = -1;

Lexer::Lexer() {
}

Lexer::~Lexer() {
}

bool Lexer::IsAlpha(const char& c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool Lexer::IsDigit(const char& c) {
  return c >= '0' && c <= '9';
}
