#ifndef LEXER_LEXER_H_
#define LEXER_LEXER_H_

#include <memory>
#include <string_view>

#include "lexer/token.h"
#include "third_party/absl/absl/status/status.h"
#include "third_party/absl/absl/status/statusor.h"
#include "third_party/googletest/googletest/include/gtest/gtest_prod.h"

class Lexer {
 public:
  static const int TYPE_END_OF_INPUT;

  Lexer() = default;
  Lexer(const Lexer&) = delete;
  Lexer& operator=(const Lexer&) = delete;
  virtual ~Lexer() = default;

  virtual absl::StatusOr<std::unique_ptr<Token>> GetToken(
      std::string_view input, int line, int column) const = 0;

 protected:
  static bool IsAlpha(char c);
  static bool IsDigit(char c);

  static absl::Status ExpectDigit(char c, int line, int column);
  static absl::Status ExpectNotControl(char c, int line, int column);
  static absl::Status ExpectNotNull(char c, int line, int column);

  static absl::Status InvalidArgumentError(char c, int line, int column);

 private:
  FRIEND_TEST(LexerTest, IsAlpha);
  FRIEND_TEST(LexerTest, IsDigit);
  FRIEND_TEST(LexerTest, ExpectDigit);
  FRIEND_TEST(LexerTest, ExpectNotControl);
  FRIEND_TEST(LexerTest, ExpectNotNull);
  FRIEND_TEST(LexerTest, UnexpectedCharacter);
};

#endif  // LEXER_LEXER_H_
