#ifndef LEXER_TOKEN_STREAM_H_
#define LEXER_TOKEN_STREAM_H_

#include <memory>
#include <string_view>

#include "absl/status/statusor.h"
#include "bonavista/lexer/lexer.h"
#include "bonavista/lexer/token.h"

class TokenStream {
 public:
  TokenStream(const Lexer& lexer, std::string_view input);
  TokenStream(const TokenStream&) = delete;
  TokenStream& operator=(const TokenStream&) = delete;
  ~TokenStream() = default;

  absl::StatusOr<std::unique_ptr<Token>> GetNextToken();

  bool HasInput() const;

 private:
  const Lexer& lexer_;

  std::string_view input_;
  int index_;
  int line_;
  int column_;
};

#endif  // LEXER_TOKEN_STREAM_H_
