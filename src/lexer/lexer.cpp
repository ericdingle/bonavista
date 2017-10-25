#include "lexer/lexer.h"

const int Lexer::TYPE_END_OF_INPUT = -1;

bool Lexer::IsAlpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool Lexer::IsDigit(char c) {
  return c >= '0' && c <= '9';
}

Status Lexer::ExpectDigit(char c, int line, int column) {
  RETURN_IF_ERROR(ExpectNotNull(c, line, column));
  return !IsDigit(c) ? UnexpectedCharacter(c, line, column) : Status();
}

Status Lexer::ExpectNotNull(char c, int line, int column) {
  return !c ? Status("Unexpected end of input", line, column) : Status();
}


Status Lexer::ExpectNotControl(char c, int line, int column) {
  RETURN_IF_ERROR(ExpectNotNull(c, line, column));
  return c < ' ' ? UnexpectedCharacter(c, line, column) : Status();
}


Status Lexer::UnexpectedCharacter(char c, int line, int column) {
  RETURN_IF_ERROR(ExpectNotNull(c, line, column));
  return Status(std::string("Unexpected character: ") + c, line, column);
}
