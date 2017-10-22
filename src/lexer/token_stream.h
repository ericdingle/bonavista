#ifndef LEXER_TOKEN_STREAM_H_
#define LEXER_TOKEN_STREAM_H_

#include <memory>
#include <string>
#include "lexer/lexer.h"
#include "lexer/token.h"
#include "util/status_or.h"

class TokenStream {
 public:
  TokenStream(const Lexer* lexer, const std::string& input);
  TokenStream(const TokenStream&) = delete;
  TokenStream& operator=(const TokenStream&) = delete;
  ~TokenStream();

  StatusOr<std::unique_ptr<Token>> GetNextToken();

  bool HasInput() const;

 private:
  const Lexer* const lexer_;

  const std::string input_;
  int index_;
  int line_;
  int column_;
};

#endif  // LEXER_TOKEN_STREAM_H_
