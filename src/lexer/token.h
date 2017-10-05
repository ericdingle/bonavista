#ifndef LEXER_TOKEN_H_
#define LEXER_TOKEN_H_

#include <string>

class Token {
 public:
  struct Position {
    int line = 1;
    int column = 1;
  };

  Token(int type, const std::string& value, const Position& position);
  Token(const Token&) = delete;
  Token& operator=(const Token&) = delete;
  ~Token();

  bool IsType(int type) const;

  int type() const;
  const std::string& value() const;
  const Position& position() const;

 private:
  const int type_;
  const std::string value_;
  const Position position_;
};

#endif  // LEXER_TOKEN_H_
