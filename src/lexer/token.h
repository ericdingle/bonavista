#ifndef LEXER_TOKEN_H_
#define LEXER_TOKEN_H_

#include <string_view>

class Token {
 public:
  Token(int type, std::string_view value, int line, int column, int length=0);

  Token(const Token&) = default;
  Token& operator=(const Token&) = delete;
  ~Token();

  bool IsType(int type) const;

  int type() const;
  std::string_view value() const;
  int line() const;
  int column() const;
  int length() const;

 private:
  int type_;
  std::string_view value_;
  int line_;
  int column_;

  // Length of the token if the length of value_ is different from the length of
  // the token in the original buffer.
  int length_;
};

#endif  // LEXER_TOKEN_H_
