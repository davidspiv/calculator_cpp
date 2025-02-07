#include <cmath>
#include <deque>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "../include/io.h"
#include "../include/math.h"

using namespace std;

enum TokenType { value, unaryOp, binaryOp, parenthesis };

struct Token {
  string symbol;
  TokenType type;
};

unordered_map<string, size_t> opRank = {{"(", 0},   {"+", 1},   {"-", 1},
                                        {"*", 2},   {"/", 2},   {"^", 3},
                                        {"NEG", 4}, {"SIN", 5}, {"COS", 5}};

bool isNumeric(const string &symbol) {
  if (!symbol.empty() && (isdigit(symbol[0]) || symbol[0] == '.')) {
    return true;
  }
  return !symbol.empty() && (isdigit(symbol[1]) || symbol[1] == '.');
}

// Determine if '-' represent negation or subtraction
bool isNegateOp(const deque<Token> &tokens) {
  return tokens.empty() ||
         (tokens.back().type != value && tokens.back().symbol != ")");
}

deque<Token> lexer(const string &inputAsString) {
  deque<Token> tokens;
  string symbolBuffer = "";

  for (size_t i = 0; i < inputAsString.length(); i++) {
    const string symbol(1, inputAsString[i]);

    if (isNumeric(symbol)) {
      symbolBuffer += inputAsString[i];
    }

    if (!isNumeric(symbol)) {
      // process numeric buffer first
      if (!symbolBuffer.empty()) {
        tokens.push_back({symbolBuffer, value});
        symbolBuffer.clear();
      }

      // process operators and parenthesis
      if (symbol == "s") {
        tokens.push_back({"SIN", unaryOp});
        i += 2;

      } else if (symbol == "c") {
        tokens.push_back({"COS", unaryOp});
        i += 2;

      } else if (symbol == "(" || symbol == ")") {
        tokens.push_back({symbol, parenthesis});

      } else if (symbol == "-" && isNegateOp(tokens)) {
        tokens.push_back({"NEG", unaryOp});

      } else if (symbol == "-") {
        tokens.push_back({symbol, binaryOp});

      } else if (opRank.find(symbol) != opRank.end()) {
        tokens.push_back({symbol, binaryOp});

      } else if (symbol != " ") {
        throw invalid_argument("ERROR: unrecognized symbol \"" + symbol + "\"");
      }
    }
  }

  // flush remaining numeric values
  if (!symbolBuffer.empty()) {
    tokens.push_back({symbolBuffer, value});
  }
  return tokens;
}

size_t getPreviousOpRank(const stack<Token> &opStack) {
  if (!opStack.empty()) {
    return opRank.at(opStack.top().symbol);
  }
  return 0;
}

deque<Token> shuntingYard(deque<Token> inputQueue) {
  stack<Token> opStack;
  deque<Token> outputQueue;

  for (Token &token : inputQueue) {
    if (token.type == value) {
      outputQueue.push_back(token);
      continue;
    }

    // flush opStack until matching parenthesis is reached
    if (token.symbol == ")") {
      while (opStack.top().symbol != "(") {
        outputQueue.push_back(opStack.top());
        opStack.pop();
      }
      opStack.pop();  // remove open parenthesis
      continue;
    }

    const size_t currentOpRank = opRank.at(token.symbol);
    size_t previousOpRank = getPreviousOpRank(opStack);

    while (token.symbol != "(" && currentOpRank <= previousOpRank) {
      // nested exponents are unique - must be evaluated right to left
      if (token.symbol == "^" && currentOpRank == previousOpRank) {
        break;
      }

      outputQueue.push_back(opStack.top());
      opStack.pop();
      previousOpRank = getPreviousOpRank(opStack);
    }

    opStack.push(token);
  }

  // flush remaining operators
  while (!opStack.empty()) {
    outputQueue.push_back(opStack.top());
    opStack.pop();
  }

  return outputQueue;
}

double evalRpnNotation(const deque<Token> &rpnNotation) {
  stack<double> result;

  for (const Token &token : rpnNotation) {
    // handle numerics
    if (token.type == value) {
      result.push(stod(token.symbol));
      continue;
    }

    // handle unary operators
    if (result.empty()) {
      throw invalid_argument("ERROR: invalid expression involving operator \"" +
                             token.symbol + " \"");
    }
    const double operandB = result.top();
    result.pop();

    if (token.symbol == "NEG") {
      result.push(operandB * -1);
      continue;
    }

    if (token.symbol == "SIN") {
      result.push(_sin(operandB));
      continue;
    } else if (token.symbol == "COS") {
      result.push(_cos(operandB));
      continue;
    }

    // handle binary operators
    if (result.empty()) {
      throw invalid_argument("ERROR: invalid expression involving operator \"" +
                             token.symbol + "\"");
    }
    const double operandA = result.top();
    result.pop();

    if (token.symbol == "^") {
      result.push(pow(operandA, operandB));

    } else if (token.symbol == "+") {
      result.push(operandA + operandB);

    } else if (token.symbol == "-") {
      result.push(operandA - operandB);

    } else if (token.symbol == "*") {
      result.push(operandA * operandB);

    } else if (token.symbol == "/") {
      if (!operandB) {
        throw invalid_argument("ERROR: unable to divide by zero");
      }
      result.push(operandA / operandB);
    }
  }
  return result.top();
}

int main() {
  print("Enter Expression. Type 'exit' to quit.");
  while (true) {
    const string inputAsString = getString(">> ");
    if (inputAsString == "exit") break;
    try {
      const deque<Token> algNotation = lexer(inputAsString);
      const deque<Token> rpnNotation = shuntingYard(algNotation);
      const double result = evalRpnNotation(rpnNotation);
      print(result);
    } catch (const std::exception &e) {
      std::cerr << e.what() << '\n';
    }
  }
  print("Successfully exited");
}
