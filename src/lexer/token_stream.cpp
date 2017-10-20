#include "lexer/token_stream.h"

#include <assert.h>
#include "lexer/lexer.h"

TokenStream::TokenStream(const Lexer* lexer, const std::string& input)
    : lexer_(lexer), input_(input), index_(0), line_(1), column_(1) {
}

TokenStream::~TokenStream() {
}

bool TokenStream::GetNextToken(std::unique_ptr<const Token>* token) {
  // Consume the white space.
  while (HasInput()) {
    const char& c = input_[index_];
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
    token->reset(new Token(Lexer::TYPE_END_OF_INPUT, "(end of input)", line_, column_));
    return true;
  }

  // Call into the lexer to get the token.
  int type;
  std::string value;
  int count;
  if (!lexer_->GetToken(input_, index_, &type, &value, &count, &error_))
    return false;

  token->reset(new Token(type, value, line_, column_));

  // Increment the index and position.
  index_ += count;
  column_ += count;

  return true;
}

bool TokenStream::HasInput() const {
  assert(index_ >= 0);
  return static_cast<unsigned int>(index_) < input_.length();
}

int TokenStream::line() const {
  return line_;
}

int TokenStream::column() const {
  return column_;
}

const std::string& TokenStream::error() const {
  return error_;
}
