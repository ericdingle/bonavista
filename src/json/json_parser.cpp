#include "json/json_parser.h"

#include <vector>
#include "json/json_lexer.h"
#include "parser/ast_node.h"

JsonParser::JsonParser(TokenStream* token_stream) : Parser(token_stream) {
}

StatusOr<std::unique_ptr<ASTNode>> JsonParser::Parse() {
  ASSIGN_OR_RETURN(auto root, Parser::Parse());

  if (HasInput())
    return UnexpectedToken(*look_ahead_token_);

  return std::move(root);
}

StatusOr<std::unique_ptr<ASTNode>> JsonParser::ParsePrefixToken(
    std::unique_ptr<const Token> token) {
  if (token->IsType(JsonLexer::TYPE_LEFT_BRACE))
    return ParseObject(std::move(token));
  else if (token->IsType(JsonLexer::TYPE_LEFT_BRACKET))
    return ParseArray(std::move(token));

  if (token->IsType(JsonLexer::TYPE_FALSE) ||
      token->IsType(JsonLexer::TYPE_NULL) ||
      token->IsType(JsonLexer::TYPE_NUMBER) ||
      token->IsType(JsonLexer::TYPE_STRING) ||
      token->IsType(JsonLexer::TYPE_TRUE))
    return std::unique_ptr<ASTNode>(new ASTNode(std::move(token)));

  return UnexpectedToken(*token);
}

StatusOr<std::unique_ptr<ASTNode>> JsonParser::ParseObject(
    std::unique_ptr<const Token> token) {
  // Implements:
  //  object -> '{' pairs '}'
  //  pair -> string ':' value
  //  pairs -> pair more_pairs | E
  //  more_pairs -> ',' pair more_pairs | E
  std::unique_ptr<ASTNode> node(new ASTNode(std::move(token)));

  if (!look_ahead_token_->IsType(JsonLexer::TYPE_RIGHT_BRACE)) {
    while (true) {
      ASSIGN_OR_RETURN(auto key, ParseExpression(0));
      RETURN_IF_ERROR(ExpectToken(key->token(), JsonLexer::TYPE_STRING));
      node->AddChild(std::move(key));

      RETURN_IF_ERROR(ConsumeToken(JsonLexer::TYPE_COLON));

      ASSIGN_OR_RETURN(auto value, ParseExpression(0));
      node->AddChild(std::move(value));

      if (!look_ahead_token_->IsType(JsonLexer::TYPE_COMMA))
        break;
      RETURN_IF_ERROR(ConsumeToken(JsonLexer::TYPE_COMMA));
    }
  }

  RETURN_IF_ERROR(ConsumeToken(JsonLexer::TYPE_RIGHT_BRACE));

  return std::move(node);
}

StatusOr<std::unique_ptr<ASTNode>> JsonParser::ParseArray(
    std::unique_ptr<const Token> token) {
  // Implements:
  //   array -> '[' values ']'
  //   values -> value more_values | E
  //   more_values -> ',' value more_values | E
  std::unique_ptr<ASTNode> node(new ASTNode(std::move(token)));

  if (!look_ahead_token_->IsType(JsonLexer::TYPE_RIGHT_BRACKET)) {
    while (true) {
      ASSIGN_OR_RETURN(auto value, ParseExpression(0));
      node->AddChild(std::move(value));

      if (!look_ahead_token_->IsType(JsonLexer::TYPE_COMMA))
        break;
      RETURN_IF_ERROR(ConsumeToken(JsonLexer::TYPE_COMMA));
    }
  }

  RETURN_IF_ERROR(ConsumeToken(JsonLexer::TYPE_RIGHT_BRACKET));

  return std::move(node);
}
