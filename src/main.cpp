#include <deque>
#include <stdexcept>
#include <string>

#include "../include/evalRpnNotation.h"
#include "../include/io.h"
#include "../include/lexer.h"
#include "../include/shuntingYard.h"
#include "../include/token.h"

int main() {
  print("Enter Expression. Type 'exit' to quit.");

  while (true) {
    const std::string inputAsString = getLine();

    if (inputAsString == "exit") break;

    try {
      const std::deque<Token> algNotation = lexer(inputAsString);
      const std::deque<Token> rpnNotation = shuntingYard(algNotation);
      const double result = evalRpnNotation(rpnNotation);
      print(result);

    } catch (const std::exception &e) {
      std::cerr << "ERROR: " << e.what() << '\n';
    }
  }

  print("Successfully exited");
}
