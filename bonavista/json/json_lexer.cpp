#include "bonavista/json/json_lexer.h"

#include <cstring>

#include "bonavista/util/status_macros.h"

absl::StatusOr<std::unique_ptr<Token>> JsonLexer::GetToken(
    std::string_view input, int line, int column) const {
  char c = input[0];
  if (IsAlpha(c)) {
    return GetKeywordToken(input.data(), line, column);
  } else if (IsDigit(c) || c == '-') {
    return GetNumberToken(input.data(), line, column);
  } else if (c == '"') {
    return GetStringToken(input.data(), line, column);
  }

  int t = -1;
  if (c == ':') {
    t = TYPE_COLON;
  } else if (c == ',') {
    t = TYPE_COMMA;
  } else if (c == '{') {
    t = TYPE_LEFT_BRACE;
  } else if (c == '}') {
    t = TYPE_RIGHT_BRACE;
  } else if (c == '[') {
    t = TYPE_LEFT_BRACKET;
  } else if (c == ']') {
    t = TYPE_RIGHT_BRACKET;
  }

  if (t != -1) {
    return std::make_unique<Token>(t, input.substr(0, 1), line, column);
  }

  return UnexpectedCharacterError(c, line, column);
}

absl::StatusOr<std::unique_ptr<Token>> JsonLexer::GetKeywordToken(
    const char* input, int line, int column) const {
  if (strncmp(input, "false", 5) == 0) {
    return std::make_unique<Token>(TYPE_KEYWORD, "false", line, column);
  } else if (strncmp(input, "null", 4) == 0) {
    return std::make_unique<Token>(TYPE_KEYWORD, "null", line, column);
  } else if (strncmp(input, "true", 4) == 0) {
    return std::make_unique<Token>(TYPE_KEYWORD, "true", line, column);
  }

  return UnexpectedCharacterError(*input, line, column);
}

absl::StatusOr<std::unique_ptr<Token>> JsonLexer::GetNumberToken(
    const char* input, int line, int column) const {
  const char* start = input;

  if (*input == '-') ++input;

  RETURN_IF_ERROR(ExpectDigit(*input, line, column));
  if (*input == '0') {
    ++input;
  } else {
    for (; IsDigit(*input); ++input)
      ;
  }

  if (*input == '.') {
    ++input;
    RETURN_IF_ERROR(ExpectDigit(*input, line, column));
    for (; IsDigit(*input); ++input)
      ;
  }

  if (*input == 'e' || *input == 'E') {
    ++input;

    if (*input == '-' || *input == '+') ++input;

    RETURN_IF_ERROR(ExpectDigit(*input, line, column));
    for (; IsDigit(*input); ++input)
      ;
  }

  return std::make_unique<Token>(
      TYPE_NUMBER, std::string_view(start, input - start), line, column);
}

absl::StatusOr<std::unique_ptr<Token>> JsonLexer::GetStringToken(
    const char* input, int line, int column) const {
  const char* start = input;
  ++input;

  while (true) {
    RETURN_IF_ERROR(ExpectNotControl(*input, line, column));

    char c = *input;

    if (c == '"') {
      ++input;
      break;
    }

    if (c == '\\') {
      ++input;

      c = *input;
      if (c == 'b' || c == 'f' || c == 'n' || c == 'r' || c == 't' ||
          c == '"' || c == '\\' || c == '/') {
        // Nothing to do.
      } else if (c == 'u') {
        for (int i = 0; i < 4; ++i) {
          ++input;
          RETURN_IF_ERROR(ExpectDigit(*input, line, column));
        }
      } else {
        return UnexpectedCharacterError(c, line, column);
      }
    }

    ++input;
  }

  return std::make_unique<Token>(TYPE_STRING,
                                 std::string_view(start + 1, input - start - 2),
                                 line, column, input - start);
}
