#include "lexer/lexer.h"

const int Lexer::TYPE_END_OF_INPUT = -1;

Lexer::Lexer() {
}

Lexer::~Lexer() {
}

bool Lexer::IsAlpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool Lexer::IsDigit(char c) {
  return c >= '0' && c <= '9';
}

Status Lexer::UnrecognizedToken(char c, int line, int column) {
  return Status(std::string("Unrecognized token: ") + c, line, column);
}
