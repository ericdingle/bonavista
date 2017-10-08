#include "lexer/token_stream.h"

#include <assert.h>
#include "lexer/lexer.h"

TokenStream::TokenStream(const Lexer* lexer, const std::string& input)
    : lexer_(lexer), input_(input), index_(0) {
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
        ++position_.line;
        position_.column = 1;
      } else {
        ++position_.column;
      }
    } else {
      break;
    }
  }

  // If we've reached the end, we return an end of input token.
  if (!HasInput()) {
    token->reset(new Token(Lexer::TYPE_END_OF_INPUT, "(end of input)", position_));
    return true;
  }

  // Call into the lexer to get the token.
  int type;
  std::string value;
  int count;
  if (!lexer_->GetToken(input_, index_, &type, &value, &count, &error_))
    return false;

  token->reset(new Token(type, value, position_));

  // Increment the index and position.
  index_ += count;
  position_.column += count;

  return true;
}

bool TokenStream::HasInput() const {
  assert(index_ >= 0);
  return static_cast<unsigned int>(index_) < input_.length();
}

const Token::Position& TokenStream::position() const {
  return position_;
}

const std::string& TokenStream::error() const {
  return error_;
}
