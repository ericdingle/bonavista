#include "bonavista/calc/calc_lexer.h"

#include "bonavista/util/status_macros.h"

absl::StatusOr<std::unique_ptr<Token>> CalcLexer::GetToken(
    std::string_view input, int line, int column) const {
  char c = input[0];
  if (IsDigit(c)) {
    return GetNumberToken(input.data(), line, column);
  }

  int t = -1;
  if (c == '*') {
    t = TYPE_ASTERISK;
  } else if (c == '(') {
    t = TYPE_LEFT_PARENTHESIS;
  } else if (c == '-') {
    t = TYPE_MINUS;
  } else if (c == '+') {
    t = TYPE_PLUS;
  } else if (c == ')') {
    t = TYPE_RIGHT_PARENTHESIS;
  } else if (c == '/') {
    t = TYPE_SLASH;
  }

  if (t != -1) {
    return std::make_unique<Token>(t, input.substr(0, 1), line, column);
  }

  return UnexpectedCharacterError(c, line, column);
}

absl::StatusOr<std::unique_ptr<Token>> CalcLexer::GetNumberToken(
    const char* input, int line, int column) const {
  const char* start = input;

  if (*input == '0') {
    ++input;
  } else {
    for (; IsDigit(*input); ++input);
  }

  if (*input == '.') {
    ++input;
    RETURN_IF_ERROR(ExpectDigit(*input, line, column));
    for (; IsDigit(*input); ++input);
  }

  return std::make_unique<Token>(
      TYPE_NUMBER, std::string_view(start, input - start), line, column);
}
