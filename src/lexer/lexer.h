#ifndef LEXER_LEXER_H_
#define LEXER_LEXER_H_

#include <string>

class Token;

class Lexer {
 public:
  // Lexer token type for end of input.
  static const int TYPE_END_OF_INPUT;

  Lexer();
  Lexer(const Lexer&) = delete;
  Lexer& operator=(const Lexer&) = delete;
  virtual ~Lexer();

  virtual bool GetToken(const std::string& input,
                        int index,
                        int* type,
                        std::string* value,
                        int* count,
                        std::string* error) const = 0;

 protected:
  static bool IsAlpha(const char& c);
  static bool IsDigit(const char& c);
};

#endif  // LEXER_LEXER_H_
