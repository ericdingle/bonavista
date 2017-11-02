#ifndef LEXER_TOKEN_H_
#define LEXER_TOKEN_H_

#include <string>

class Token {
 public:
  Token(int type, char c, int line, int column);
  Token(int type, const std::string& value, int line, int column, int length=0);

  Token(const Token&) = default;
  Token& operator=(const Token&) = delete;
  ~Token();

  bool IsType(int type) const;

  int type() const;
  const std::string& value() const;
  int line() const;
  int column() const;
  int length() const;

 private:
  const int type_;
  const std::string value_;
  const int line_;
  const int column_;

  // Length of the token if the length of value_ is different from the length of
  // the token in the original buffer.
  const int length_;
};

#endif  // LEXER_TOKEN_H_
