#include "lexer/token_stream.h"

#include "lexer/lexer.h"

TokenStream::TokenStream(const Lexer& lexer, std::string_view input)
    : lexer_(lexer), input_(input), index_(0), line_(1), column_(1) {
}

StatusOr<std::unique_ptr<Token>> TokenStream::GetNextToken() {
  // Consume the white space.
  while (HasInput()) {
    char c = input_[index_];
    if (c <= ' ') {
      ++index_;
      if (c == '\n') {
        ++line_;
        column_ = 1;
      } else {
        ++column_;
      }
    } else {
      break;
    }
  }

  // If we've reached the end, we return an end of input token.
  if (!HasInput()) {
    return std::make_unique<Token>(Lexer::TYPE_END_OF_INPUT, "(end of input)",
                                   line_, column_);
  }

  // Call into the lexer to get the token.
  ASSIGN_OR_RETURN(
      auto token, lexer_.GetToken(input_.substr(index_), line_, column_));

  // Increment the index and position.
  index_ += token->length();
  column_ += token->length();

  return std::move(token);
}

bool TokenStream::HasInput() const {
  return index_ < static_cast<int>(input_.length());
}
