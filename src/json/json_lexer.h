#ifndef JSON_JSON_LEXER_H_
#define JSON_JSON_LEXER_H_

#include "lexer/lexer.h"

class JsonLexer : public Lexer {
 public:
  JsonLexer();
  JsonLexer(const JsonLexer&) = delete;
  JsonLexer& operator=(const JsonLexer&) = delete;
  virtual ~JsonLexer();

  enum Type {
    TYPE_COLON,
    TYPE_COMMA,
    TYPE_FALSE,
    TYPE_LEFT_BRACE,
    TYPE_LEFT_BRACKET,
    TYPE_NULL,
    TYPE_NUMBER,
    TYPE_RIGHT_BRACE,
    TYPE_RIGHT_BRACKET,
    TYPE_TRUE,
    TYPE_STRING
  };

  virtual bool GetToken(const std::string& input,
                        int index,
                        int* type,
                        std::string* value,
                        int* count,
                        std::string* error) const;

 private:
  bool GetKeywordToken(const std::string& input,
                       int index,
                       int* type,
                       std::string* value,
                       int* count,
                       std::string* error) const;

  bool GetNumberToken(const std::string& input,
                      int index,
                      int* type,
                      std::string* value,
                      int* count,
                      std::string* error) const;

  bool GetStringToken(const std::string& input,
                      int index,
                      int* type,
                      std::string* value,
                      int* count,
                      std::string* error) const;
};

#endif  // JSON_JSON_LEXER_H_
