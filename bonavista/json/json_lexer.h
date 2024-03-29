#ifndef JSON_JSON_LEXER_H_
#define JSON_JSON_LEXER_H_

#include "bonavista/lexer/lexer.h"

class JsonLexer : public Lexer {
 public:
  JsonLexer() = default;
  JsonLexer(const JsonLexer&) = delete;
  JsonLexer& operator=(const JsonLexer&) = delete;
  ~JsonLexer() override = default;

  enum Type {
    TYPE_COLON,
    TYPE_COMMA,
    TYPE_KEYWORD,
    TYPE_LEFT_BRACE,
    TYPE_LEFT_BRACKET,
    TYPE_NUMBER,
    TYPE_RIGHT_BRACE,
    TYPE_RIGHT_BRACKET,
    TYPE_STRING
  };

  absl::StatusOr<std::unique_ptr<Token>> GetToken(std::string_view input,
                                                  int line,
                                                  int column) const override;

 private:
  absl::StatusOr<std::unique_ptr<Token>> GetKeywordToken(const char* input,
                                                         int line,
                                                         int column) const;
  absl::StatusOr<std::unique_ptr<Token>> GetNumberToken(const char* input,
                                                        int line,
                                                        int column) const;
  absl::StatusOr<std::unique_ptr<Token>> GetStringToken(const char* input,
                                                        int line,
                                                        int column) const;
};

#endif  // JSON_JSON_LEXER_H_
