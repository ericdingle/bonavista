#include <assert.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include "calc/calc_executer.h"
#include "calc/calc_lexer.h"
#include "calc/calc_parser.h"
#include "lexer/token_stream.h"

namespace {
const char* kInputPrefix = "calc > ";
}  // namespace

int main() {
  CalcLexer lexer;

  while (true) {
    printf("%s", kInputPrefix);
    std::string input;
    getline(std::cin, input);
    if (input.empty())
      break;

    TokenStream stream(lexer, input);
    CalcParser parser(&stream);
    CalcExecuter executer(&parser);

    auto status_or = executer.Execute();
    if (!status_or.ok()) {
      auto msg = status_or.status().message();
      printf("Error: %.*s.\n", static_cast<int>(msg.length()), msg.data());
      continue;
    }

    double result = std::any_cast<double>(status_or.value());;
    printf("%f\n", result);
  }

  return 0;
}
