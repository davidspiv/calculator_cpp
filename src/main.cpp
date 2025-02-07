#include <cmath>
#include <deque>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "../include/io.h"
#include "../include/taylorSeries.h"

using namespace std;

enum TokenType { value, unaryOp, binaryOp };

struct Token {
  string symbol;
  TokenType type;
};

bool isNumeric(const string &symbol) {
  if (!symbol.empty() && (isdigit(symbol[0]) || symbol[0] == '.')) {
    return true;
  }
  return symbol.size() > 1 && (isdigit(symbol[1]) || symbol[1] == '.');
}

deque<Token> lexer(const string &inputAsString) {
  deque<Token> tokens;
  string valueTokenBuffer = "";

  for (size_t i = 0; i < inputAsString.length(); i++) {
    const string symbol(1, inputAsString[i]);
    const size_t lastIndex = inputAsString.length() - 1;

    if (isNumeric(symbol)) {
      valueTokenBuffer += inputAsString[i];
    }

    if (!isNumeric(symbol) || i == lastIndex) {
      if (!valueTokenBuffer.empty()) {
        tokens.push_back({valueTokenBuffer, value});
        valueTokenBuffer.clear();
      }

      const bool isNegative =
          (symbol == "-") && (tokens.empty() || (tokens.back().type != value &&
                                                 tokens.back().symbol != ")"));

      if (isNegative) {
        tokens.push_back({"NEG", unaryOp});
      } else if (symbol == "s") {
        tokens.push_back({"SIN", unaryOp});
        i += 2;
      } else if (symbol == "c") {
        tokens.push_back({"COS", unaryOp});
        i += 2;
      } else if (symbol != " " && !isNumeric(symbol)) {
        tokens.push_back({symbol, binaryOp});
      }
    }
  }
  return tokens;
}

deque<Token> shuntingYard(deque<Token> inputQueue) {
  stack<Token> opStack;
  deque<Token> outputQueue;
  unordered_map<string, size_t> opRank = {{"(", 0},   {"+", 1},   {"-", 1},
                                          {"*", 2},   {"/", 2},   {"^", 3},
                                          {"NEG", 4}, {"SIN", 5}, {"COS", 5}};

  for (Token &token : inputQueue) {
    if (token.type == value) {
      outputQueue.push_back(token);
      continue;
    }

    if (token.symbol == ")") {
      while (opStack.top().symbol != "(") {
        outputQueue.push_back(opStack.top());
        opStack.pop();
      }
      opStack.pop();
      continue;
    }

    while (token.symbol != "(" && !opStack.empty() &&
           opRank.at(token.symbol) <= opRank.at(opStack.top().symbol)) {
      outputQueue.push_back(opStack.top());
      opStack.pop();
    }

    opStack.push(token);
  }

  while (!opStack.empty()) {
    outputQueue.push_back(opStack.top());
    opStack.pop();
  }

  return outputQueue;
}

double evalRpnNotation(const deque<Token> &rpnNotation) {
  stack<double> result;

  for (const Token &token : rpnNotation) {
    if (token.type == value) {
      result.push(stod(token.symbol));
    } else {
      const double operandB = result.top();
      result.pop();

      if (token.symbol == "NEG") {
        result.push(operandB * -1);
        continue;
      }

      if (token.symbol == "SIN" || token.symbol == "COS") {
        const double normalizedRadians = normalizeRadians(operandB);
        result.push(taylorSeries(token.symbol, normalizedRadians));
        continue;
      }

      const double operandA = result.top();
      result.pop();

      if (token.symbol == "^") {
        double eval = operandA;
        for (size_t i = 1; i < abs(operandB); i++) {
          eval *= operandA;
        }
        if (operandB < 0) {
          result.push(1 / eval);
        } else {
          result.push(eval);
        }
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
      } else {
        throw invalid_argument("ERROR: unrecognized symbol");
      }
    }
  }
  return result.top();
}

int main() {
  //   const string inputAsString = getString("Enter Expression: ");
  const string inputAsString = "-sin(4^2 / -(3 - -3^2) / -cos(.2))";
  const deque<Token> algNotation = lexer(inputAsString);

  const deque<Token> rpnNotation = shuntingYard(algNotation);
  const double result = evalRpnNotation(rpnNotation);
  print(result, "Answer: ");

  // TEST
  const double test = -sin(pow(4, 2) / -(3 - pow(-3, 2)) / -cos(.2));
  print(test, "  Test: ");
}
