#ifndef LEXER_LEXER_H_
#define LEXER_LEXER_H_

#include <memory>
#include <string>
#include "util/status_or.h"

class Token;

class Lexer {
 public:
  static const int TYPE_END_OF_INPUT;

  Lexer();
  Lexer(const Lexer&) = delete;
  Lexer& operator=(const Lexer&) = delete;
  virtual ~Lexer();

  virtual StatusOr<std::unique_ptr<Token>> GetToken(
      const char* input, int line, int column) const = 0;

 protected:
  static bool IsAlpha(char c);
  static bool IsDigit(char c);

  static Status UnrecognizedToken(char c, int line, int column);
};

#endif  // LEXER_LEXER_H_
