#include <deque>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "../include/_math.h"
#include "../include/io.h"
#include "../include/taylorSeries.h"
#include "../include/token.h"
#include "../include/operators.h"
#include "../include/helpers.h"

std::deque<Token> lexer(const std::string &inputAsString) {
  std::deque<Token> tokens;
  std::string valueBuff = "";
  std::string opSymbolBuff = "";

  for (size_t i = 0; i < inputAsString.length(); i++) {
    const char symbol = inputAsString[i];
    const std::string symbolAsString = std::string(1, symbol);

    // handle numeric buffer for multi-char values
    if (isNumeric(symbol)) {
      valueBuff += symbol;
      continue;
    }

    // create value token
    if (!valueBuff.empty()) {
      tokens.push_back({valueBuff, value});
      valueBuff.clear();
    }

    // handle operator buffer for multi-char names
    if (isalpha(symbol)) {
      opSymbolBuff += symbol;
      if (opSymbolBuff.length() < 3) {
        continue;
      }
      if (opRank.find(opSymbolBuff) == opRank.end()) {
        throw std::invalid_argument("unrecognized function \"" + opSymbolBuff +
                                    "\".");
      }
    }

    // create non-numeric token
    if (!opSymbolBuff.empty()) {
      tokens.push_back({opSymbolBuff, unaryOp});
      opSymbolBuff.clear();

    } else if (symbol == '(' || symbol == ')') {
      tokens.push_back({symbolAsString, parenthesis});

    } else if (symbol == '-' && isNegateOp(tokens)) {
      tokens.push_back({"neg", unaryOp});

    } else if (symbol == '-') {
      tokens.push_back({symbolAsString, binaryOp});

    } else if (opRank.find(symbolAsString) != opRank.end()) {
      tokens.push_back({symbolAsString, binaryOp});

    } else if (symbol != ' ') {
      throw std::invalid_argument("unrecognized symbol \"" + symbolAsString +
                                  "\".");
    }
  }

  // flush remaining values
  if (!valueBuff.empty()) {
    tokens.push_back({valueBuff, value});
  }

  return tokens;
}

std::deque<Token> shuntingYard(std::deque<Token> inputQueue) {
  std::stack<Token> opStack;
  std::deque<Token> outputQueue;

  for (Token &token : inputQueue) {
    if (token.type == value) {
      outputQueue.push_back(token);
      continue;
    }

    // flush opStack until matching parenthesis is reached
    if (token.symbol == ")") {
      if (opStack.empty()) {
        throw std::invalid_argument("mismatched parentheses.");
      }
      while (opStack.top().symbol != "(") {
        outputQueue.push_back(opStack.top());
        opStack.pop();
        if (opStack.empty()) {
          throw std::invalid_argument("mismatched parentheses.");
        }
      }
      opStack.pop();  // remove open parenthesis
      continue;
    }

    const size_t currentOpRank = opRank.at(token.symbol);
    size_t previousOpRank = safePeekTop(opStack);

    while (token.symbol != "(" && currentOpRank <= previousOpRank) {
      // nested exponents are unique - must be evaluated right to left
      if (token.symbol == "^" && currentOpRank == previousOpRank) {
        break;
      }

      outputQueue.push_back(opStack.top());
      opStack.pop();
      previousOpRank = safePeekTop(opStack);
    }

    opStack.push(token);
  }

  // flush remaining operators
  while (!opStack.empty()) {
    if (opStack.top().type == parenthesis) {
      throw std::invalid_argument("mismatched parentheses.");
    }
    outputQueue.push_back(opStack.top());
    opStack.pop();
  }

  return outputQueue;
}

double evalRpnNotation(const std::deque<Token> &rpnNotation) {
  std::stack<double> result;

  for (const Token &token : rpnNotation) {
    // handle numerics
    if (token.type == value) {
      result.push(stod(token.symbol));
      continue;
    }

    // handle unary operators
    if (result.empty()) {
      throw std::invalid_argument("invalid expression involving operator \"" +
                                  token.symbol + " \".");
    }
    const double operandB = result.top();
    result.pop();

    if (token.symbol == "neg") {
      result.push(operandB * -1);
      continue;
    }

    if (token.type == unaryOp) {
      result.push(taylorSeries(token, operandB));
      continue;
    }

    // handle binary operators
    if (result.empty()) {
      throw std::invalid_argument("invalid expression involving operator \"" +
                                  token.symbol + "\".");
    }

    const double operandA = result.top();
    result.pop();

    if (token.symbol == "^") {
      result.push(_pow(operandA, operandB));

    } else if (token.symbol == "+") {
      result.push(operandA + operandB);

    } else if (token.symbol == "-") {
      result.push(operandA - operandB);

    } else if (token.symbol == "*") {
      result.push(operandA * operandB);

    } else if (token.symbol == "/") {
      if (!operandB) {
        throw std::invalid_argument("unable to divide by zero.");
      }
      result.push(operandA / operandB);
    }
  }
  return result.top();
}

int main() {
  print("Enter Expression. Type 'exit' to quit.");
  while (true) {
    const std::string inputAsString = getString(">> ");
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
