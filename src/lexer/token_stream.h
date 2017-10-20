#ifndef LEXER_TOKEN_STREAM_H_
#define LEXER_TOKEN_STREAM_H_

#include <memory>
#include <string>
#include "lexer/token.h"

class Lexer;

class TokenStream {
 public:
  TokenStream(const Lexer* lexer, const std::string& input);
  TokenStream(const TokenStream&) = delete;
  TokenStream& operator=(const TokenStream&) = delete;
  ~TokenStream();

  bool GetNextToken(std::unique_ptr<const Token>* token);

  bool HasInput() const;

  int line() const;
  int column() const;
  const std::string& error() const;

 private:
  const Lexer* const lexer_;

  const std::string input_;
  int index_;

  int line_;
  int column_;
  std::string error_;
};

#endif  // LEXER_TOKEN_STREAM_H_
