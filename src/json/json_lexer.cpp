#include "json/json_lexer.h"

#include <cstring>

StatusOr<std::unique_ptr<Token>> JsonLexer::GetToken(
    const char* input, int line, int column) const {
  char c = *input;
  if (IsAlpha(c)) {
    return GetKeywordToken(input, line, column);
  } else if (IsDigit(c) || c == '-') {
    return GetNumberToken(input, line, column);
  } else if (c == '"') {
    return GetStringToken(input, line, column);
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
    return std::unique_ptr<Token>(new Token(t, c, line, column));
  }

  return UnexpectedCharacter(c, line, column);
}

StatusOr<std::unique_ptr<Token>> JsonLexer::GetKeywordToken(
    const char* input, int line, int column) const {
  if (strncmp(input, "false", 5) == 0) {
    return std::unique_ptr<Token>(new Token(TYPE_FALSE, "false", line, column));
  } else if (strncmp(input, "null", 4) == 0) {
    return std::unique_ptr<Token>(new Token(TYPE_NULL, "null", line, column));
  } else if (strncmp(input, "true", 4) == 0) {
    return std::unique_ptr<Token>(new Token(TYPE_TRUE, "true", line, column));
  }

  return UnexpectedCharacter(*input, line, column);
}

StatusOr<std::unique_ptr<Token>> JsonLexer::GetNumberToken(
    const char* input, int line, int column) const {
  const char* start = input;

  if (*input == '-')
    ++input;

  RETURN_IF_ERROR(ExpectDigit(*input, line, column));
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

  if (*input == 'e' || *input == 'E') {
    ++input;

    if (*input == '-' || *input == '+')
      ++input;

    RETURN_IF_ERROR(ExpectDigit(*input, line, column));
    for (; IsDigit(*input); ++input);
  }

  return std::unique_ptr<Token>(new Token(
      TYPE_NUMBER, std::string(start, input - start), line, column));
}

StatusOr<std::unique_ptr<Token>> JsonLexer::GetStringToken(
    const char* input, int line, int column) const {
  const char* start = input;
  std::string str;
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
      if (c == 'b') {
        c = '\b';
      } else if (c == 'f') {
        c = '\f';
      } else if (c == 'n') {
        c = '\n';
      } else if (c == 'r') {
        c = '\r';
      } else if (c == 't') {
        c = '\t';
      } else if (c == 'u') {
        c = '?';
        for (int i = 0; i < 4; ++i) {
          ++input;
          RETURN_IF_ERROR(ExpectDigit(*input, line, column));
        }
      } else if (c == '"' || c == '\\' || c == '/') {
        // Nothing to do.
      } else {
        return UnexpectedCharacter(c, line, column);
      }
    }

    str += c;
    ++input;
  }

  return std::unique_ptr<Token>(
      new Token(TYPE_STRING, str, line, column, input - start));
}
