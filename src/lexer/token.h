#ifndef LEXER_TOKEN_H_
#define LEXER_TOKEN_H_

#include <string>

class Token {
 public:
  Token(int type, const std::string& value, int line, int column);
  Token(const Token&) = delete;
  Token& operator=(const Token&) = delete;
  ~Token();

  bool IsType(int type) const;

  int type() const;
  const std::string& value() const;
  int line() const;
  int column() const;

 private:
  const int type_;
  const std::string value_;
  const int line_;
  const int column_;
};

#endif  // LEXER_TOKEN_H_
