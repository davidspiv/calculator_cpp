#include <deque>
#include <iostream>
#include <stack>

#include "../include/_math.h"
#include "../include/taylorSeries.h"
#include "../include/token.h"

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
