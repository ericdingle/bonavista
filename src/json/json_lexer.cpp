#include "json/json_lexer.h"

JsonLexer::JsonLexer() {
}

JsonLexer::~JsonLexer() {
}

bool JsonLexer::GetToken(const std::string& input,
                         int index,
                         int* type,
                         std::string* value,
                         int* count,
                         std::string* error) const {
  const char& c = input[index];
  if (IsAlpha(c))
    return GetKeywordToken(input, index, type, value, count, error);
  else if (IsDigit(c) || c == '-')
    return GetNumberToken(input, index, type, value, count, error);
  else if (c == '"')
    return GetStringToken(input, index, type, value, count, error);

  int t = -1;
  if (c == ':')
    t = TYPE_COLON;
  else if (c == ',')
    t = TYPE_COMMA;
  else if (c == '{')
    t = TYPE_LEFT_BRACE;
  else if (c == '}')
    t = TYPE_RIGHT_BRACE;
  else if (c == '[')
    t = TYPE_LEFT_BRACKET;
  else if (c == ']')
    t = TYPE_RIGHT_BRACKET;

  if (t != -1) {
    *type = t;
    *value = c;
    *count = 1;
    return true;
  }

  *error = std::string("Unrecognized token: ") + c;
  return false;
}

bool JsonLexer::GetKeywordToken(const std::string& input,
                                int index,
                                int* type,
                                std::string* value,
                                int* count,
                                std::string* error) const {
  if (input.compare(index, 5, "false") == 0) {
    *type = TYPE_FALSE;
    *value = "false";
    *count = 5;
    return true;
  } else if (input.compare(index, 4, "null") == 0) {
    *type = TYPE_NULL;
    *value = "null";
    *count = 4;
    return true;
  } else if (input.compare(index, 4, "true") == 0) {
    *type = TYPE_TRUE;
    *value = "true";
    *count = 4;
    return true;
  }

  *error = "Unrecognized token";
  return false;
}

bool JsonLexer::GetNumberToken(const std::string& input,
                               int index,
                               int* type,
                               std::string* value,
                               int* count,
                               std::string* error) const {
  const int length = input.length();
  const int start = index;

  if (input[index] == '-')
    ++index;

  if (index == length) {
    *error = "Unexpected end of input";
    return false;
  } else if (!IsDigit(input[index])) {
    *error = "Expecting digit";
    return false;
  }

  if (input[index] == '0') {
    ++index;
    if (index < length && IsDigit(input[index])) {
      *error = "Unexpected digit";
      return false;
    }
  } else
    for (; index < length && IsDigit(input[index]); ++index);

  if (index < length && input[index] == '.') {
    ++index;

    if (index == length) {
      *error = "Unexpected end of input";
      return false;
    } else if (!IsDigit(input[index])) {
      *error = "Expecting digit";
      return false;
    }

    for (; index < length && IsDigit(input[index]); ++index);
  }

  if (index < length && (input[index] == 'e' || input[index] == 'E')) {
    ++index;

    if (index < length && (input[index] == '-' || input[index] == '+'))
      ++index;

    if (index == length) {
      *error = "Unexpected end of input";
      return false;
    } else if (!IsDigit(input[index])) {
      *error = "Expecting digit";
      return false;
    }

    for (; index < length && IsDigit(input[index]); ++index);
  }

  *type = TYPE_NUMBER;
  *value = input.substr(start, index - start);
  *count = index - start;
  return true;
}

bool JsonLexer::GetStringToken(const std::string& input,
                               int index,
                               int* type,
                               std::string* value,
                               int* count,
                               std::string* error) const {
  const int length = input.length();
  const int start = index;
  std::string str;

  ++index;

  while (true) {
    if (index == length) {
      *error = "Unterminated string";
      return false;
    }

    char c = input[index];

    if (c == '"') {
      ++index;
      break;
    }

    if (c < ' ') {
      *error = "Unterminated string";
      return false;
    }

    if (c == '\\') {
      ++index;
      if (index == length) {
        *error = "Unterminated string";
        return false;
      }

      c = input[index];
      if (c == 'b')
        c = '\b';
      else if (c == 'f')
        c = '\f';
      else if (c == 'n')
        c = '\n';
      else if (c == 'r')
        c = '\r';
      else if (c == 't')
        c = '\t';
      else if (c == 'u') {
        c = '?';
        for (int i = 0; i < 4; ++i) {
          ++index;
          if (index == length) {
            *error = "Unexpected end of input";
            return false;
          } else if (!IsDigit(input[index])) {
            *error = "Expecting digit";
            return false;
          }
        }
      } else if (c == '"' || c == '\\' || c == '/') {
        // Nothing to do.
      } else {
        *error = "Unrecognized escape sequence";
        return false;
      }
    }

    str += c;
    ++index;
  }

  *type = TYPE_STRING;
  *value = str;
  *count = index - start;
  return true;
}
