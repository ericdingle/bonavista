#ifndef LEXER_LEXER_TEST_FIXTURE_H_
#define LEXER_LEXER_TEST_FIXTURE_H_

#include <memory>
#include "lexer/token.h"
#include "third_party/googletest/googletest/include/gtest/gtest.h"
#include "third_party/absl/absl/status/statusor.h"

template <typename LexerT>
class LexerTestFixture : public testing::Test {
 protected:
  absl::StatusOr<std::unique_ptr<Token>> GetToken(const char* input) {
    return lexer_.GetToken(input, 1, 2);
  }

 private:
  LexerT lexer_;
};

#endif  // LEXER_LEXER_TEST_FIXTURE_H_
