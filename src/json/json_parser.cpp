#include "json/json_parser.h"

#include <assert.h>
#include <vector>
#include "json/json_lexer.h"
#include "parser/ast_node.h"

JsonParser::JsonParser(TokenStream* token_stream) : Parser(token_stream) {
}

JsonParser::~JsonParser() {
}

bool JsonParser::Parse(std::unique_ptr<const ASTNode>* root) {
  std::unique_ptr<const ASTNode> node;
  if (!Parser::Parse(&node))
    return false;
  if (!node.get()) {
    error_ = "Input is empty.";
    return false;
  }

  std::unique_ptr<const ASTNode> dummy;
  if (!Parser::Parse(&dummy) || dummy.get()) {
    error_ = "Encountered more than one expression.";
    return false;
  }

  *root = std::move(node);
  return true;
}

bool JsonParser::ParsePrefixToken(std::unique_ptr<const Token> token,
                                  std::unique_ptr<const ASTNode>* root) {
  if (token->IsType(JsonLexer::TYPE_LEFT_BRACE))
    return ParseObject(std::move(token), root);
  else if (token->IsType(JsonLexer::TYPE_LEFT_BRACKET))
    return ParseArray(std::move(token), root);

  if (token->IsType(JsonLexer::TYPE_FALSE) ||
      token->IsType(JsonLexer::TYPE_NULL) ||
      token->IsType(JsonLexer::TYPE_NUMBER) ||
      token->IsType(JsonLexer::TYPE_STRING) ||
      token->IsType(JsonLexer::TYPE_TRUE)) {
    root->reset(new ASTNode(std::move(token)));
    return true;
  }

  line_ = token->line();
  column_ = token->column();
  error_ = "Unexpected token: " + token->value();
  return false;
}

bool JsonParser::ParseObject(std::unique_ptr<const Token> token,
                             std::unique_ptr<const ASTNode>* root) {
  // Implements:
  //  object -> '{' pairs '}'
  //  pair -> string ':' value
  //  pairs -> pair more_pairs | E
  //  more_pairs -> ',' pair more_pairs | E
  std::unique_ptr<ASTNode> node(new ASTNode(std::move(token)));

  if (!look_ahead_token_->IsType(JsonLexer::TYPE_RIGHT_BRACE)) {
    while (true) {
      std::unique_ptr<const ASTNode> key;
      if (!ParseExpression(0, &key))
        return false;

      if (!key->token()->IsType(JsonLexer::TYPE_STRING)) {
        line_ = key->token()->line();
        column_ = key->token()->column();
        error_ = "Expecting string but found " + key->token()->value();
        return false;
      }

      node->AddChild(std::move(key));

      if (!ConsumeToken(JsonLexer::TYPE_COLON))
        return false;

      std::unique_ptr<const ASTNode> value;
      if (!ParseExpression(0, &value))
        return false;

      node->AddChild(std::move(value));

      if (!look_ahead_token_->IsType(JsonLexer::TYPE_COMMA))
        break;

      assert(ConsumeToken(JsonLexer::TYPE_COMMA));
    }
  }

  if (!ConsumeToken(JsonLexer::TYPE_RIGHT_BRACE))
    return false;

  *root = std::move(node);
  return true;
}

bool JsonParser::ParseArray(std::unique_ptr<const Token> token,
                            std::unique_ptr<const ASTNode>* root) {
  // Implements:
  //   array -> '[' values ']'
  //   values -> value more_values | E
  //   more_values -> ',' value more_values | E
  std::unique_ptr<ASTNode> node(new ASTNode(std::move(token)));

  if (!look_ahead_token_->IsType(JsonLexer::TYPE_RIGHT_BRACKET)) {
    while (true) {
      std::unique_ptr<const ASTNode> value;
      if (!ParseExpression(0, &value))
        return false;

      node->AddChild(std::move(value));

      if (!look_ahead_token_->IsType(JsonLexer::TYPE_COMMA))
        break;

      assert(ConsumeToken(JsonLexer::TYPE_COMMA));
    }
  }

  if (!ConsumeToken(JsonLexer::TYPE_RIGHT_BRACKET))
    return false;

  *root = std::move(node);
  return true;
}
