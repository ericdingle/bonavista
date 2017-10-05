#include <fstream>
#include <stdio.h>
#include <string>
#include "json/json_lexer.h"
#include "json/json_parser.h"
#include "lexer/token_stream.h"
#include "parser/ast_node.h"

void PrintJsonTree(const ASTNode* node, int level);

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage: json <filename>\n");
    return 1;
  }

  std::ifstream input(argv[1]);
  std::string buffer((std::istreambuf_iterator<char>(input)),
                     (std::istreambuf_iterator<char>()));

  JsonLexer lexer;
  TokenStream stream(&lexer, buffer.c_str());

  JsonParser parser(&stream);
  std::unique_ptr<const ASTNode> root;
  if (!parser.Parse(&root)) {
    printf("Error: %s at line %d, column %d\n", parser.error().c_str(),
           parser.position().line, parser.position().column);
    return 1;
  }

  PrintJsonTree(root.get(), 0);
  printf("\n");

  return 0;
}

void PrintJsonTree(const ASTNode* node, int level) {
  std::string indent(2 * level, ' ');

  if (node->token()->IsType(JsonLexer::TYPE_LEFT_BRACE)) {
    printf("{");
    const std::vector<std::unique_ptr<const ASTNode> >& children = node->children();
    const char* comma = "";
    for(uint i = 0; i < children.size(); i += 2) {
      printf("%s\n%s  %s: ", comma, indent.c_str(),
             children[i]->token()->value().c_str());
      comma = ",";
      PrintJsonTree(children[i + 1].get(), level + 1);
    }
    printf("\n%s}", indent.c_str());
    return;
  }

  if (node->token()->IsType(JsonLexer::TYPE_LEFT_BRACKET)) {
    printf("[");
    const std::vector<std::unique_ptr<const ASTNode> >& children = node->children();
    const char* comma = "";
    for(uint i = 0; i < children.size(); ++i) {
      printf("%s\n%s  ", comma, indent.c_str());
      comma = ",";
      PrintJsonTree(children[i].get(), level + 1);
    }
    printf("\n%s]", indent.c_str());
    return;
  }

  if (node->token()->IsType(JsonLexer::TYPE_STRING)) {
    printf("\"%s\"", node->token()->value().c_str());
    return;
  }

  printf("%s", node->token()->value().c_str());
}