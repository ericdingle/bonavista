#include "lexer/lexer.h"

#include "absl/strings/str_cat.h"
#include "util/error.h"

const int Lexer::TYPE_END_OF_INPUT = -1;

bool Lexer::IsAlpha(char c) {
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool Lexer::IsDigit(char c) { return c >= '0' && c <= '9'; }

absl::Status Lexer::ExpectDigit(char c, int line, int column) {
  return !IsDigit(c) ? UnexpectedCharacterError(c, line, column) : absl::OkStatus();
}

absl::Status Lexer::ExpectNotNull(char c, int line, int column) {
  return !c ? UnexpectedCharacterError(c, line, column) : absl::OkStatus();
}

absl::Status Lexer::ExpectNotControl(char c, int line, int column) {
  return c < ' ' ? UnexpectedCharacterError(c, line, column) : absl::OkStatus();
}

absl::Status Lexer::UnexpectedCharacterError(char c, int line, int column) {
  return Error(absl::StrCat(
      "Unexpected character '",
      c ? std::string(1, c) : "(end of input)", "'"), line,
      column);
}
