#include <deque>
#include <list>
#include <stdexcept>
#include <string>

#include "../include/evalRpnNotation.h"
#include "../include/io.h"
#include "../include/lexer.h"
#include "../include/shuntingYard.h"
#include "../include/token.h"

int main() {
  print("Enter Expression. Type 'exit' to quit.");
  std::list<std::string> history;

  while (true) {
    const std::string inputAsString = getString(history);

    if (inputAsString == "exit") break;

    try {
      const std::deque<Token> algNotation = lexer(inputAsString);
      history.push_back(inputAsString);
      const std::deque<Token> rpnNotation = shuntingYard(algNotation);
      const double result = evalRpnNotation(rpnNotation);
      print(result);

    } catch (const std::exception &e) {
      std::cerr << "ERROR: " << e.what() << '\n';
    }
  }

  print("Successfully exited");
}
