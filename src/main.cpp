#include <deque>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "../include/io.h"

using namespace std;

enum tokenType { value, unaryOp, binaryOp };

struct Token {
  string text;
  tokenType type;
};

bool isNumeric(string token) {
  if (!token.empty() && (isdigit(token[0]) || token[0] == '.')) {
    return true;
  }
  return token.size() > 1 && (isdigit(token[1]) || token[1] == '.');
}

// Removes unary negation op (-) and whitespace
deque<string> tokenize(const string &inputAsString) {
  deque<string> tokens;
  string valueTokenBuffer = "";

  for (size_t i = 0; i < inputAsString.length(); i++) {
    const string symbol(1, inputAsString[i]);
    const size_t lastIndex = inputAsString.length() - 1;

    if (isNumeric(symbol)) {
      valueTokenBuffer += inputAsString[i];
    }

    if (!isNumeric(symbol) || i == lastIndex) {
      if (!valueTokenBuffer.empty()) {
        tokens.push_back(valueTokenBuffer);
        valueTokenBuffer.clear();
      }

      if ((symbol == "-" && tokens.empty()) ||
          (symbol == "-" && (tokens.empty() || (!isNumeric(tokens.back()) &&
                                                tokens.back() != ")")))) {
        valueTokenBuffer = '-';
      } else if (symbol != " " && (!isNumeric(symbol) || symbol == ")")) {
        tokens.push_back(symbol);
      }
    }
  }
  return tokens;
}

deque<string> shuntingYard(deque<string> inputQueue) {
  stack<char> opStack;
  deque<string> outputQueue;
  unordered_map<char, size_t> opRank;
  opRank['+'] = 1;
  opRank['-'] = 1;
  opRank['*'] = 2;
  opRank['/'] = 2;
  opRank['('] = 0;

  for (string &token : inputQueue) {
    if (isNumeric(token)) {
      outputQueue.push_back(token);
      continue;
    }

    const char opAsChar = token[0];

    if (opAsChar == ')') {
      while (opStack.top() != '(') {
        outputQueue.push_back(string(1, opStack.top())), opStack.pop();
      }
      opStack.pop();
      continue;
    }

    while (opAsChar != '(' && !opStack.empty() &&
           opRank.at(opAsChar) < opRank.at(opStack.top())) {
      outputQueue.push_back(string(1, opStack.top())), opStack.pop();
    }

    opStack.push(opAsChar);
  }

  while (!opStack.empty()) {
    outputQueue.push_back(string(1, opStack.top())), opStack.pop();
  }

  return outputQueue;
}

double evalRpnNotation(deque<string> rpnNotation) {
  stack<double> result;

  for (const string &token : rpnNotation) {
    if (isNumeric(token) || (token.length() > 1 && isNumeric(token))) {
      result.push(stod(token));
    } else {
      const double operandB = result.top();
      result.pop();
      const double operandA = result.top();
      result.pop();

      switch (token[0]) {
        case '+':
          result.push(operandA + operandB);
          break;
        case '-':
          result.push(operandA - operandB);
          break;
        case '*':
          result.push(operandA * operandB);
          break;
        case '/':
          if (!operandB && operandA) {
            throw invalid_argument("ERROR: unable to divide by zero");
          }
          result.push(operandA / operandB);
          break;

        default:
          throw invalid_argument("ERROR: unrecognized non-numeric");
      }
    }
  }
  return result.top();
}

bool validAlgNotation(deque<string> algNotation) {
  int openParentheses = 0;
  size_t previousNumeric = 0;
  size_t previousOp = 0;
  string errorMessage;

  if (algNotation.empty()) {
    errorMessage = "empty input";
  } else if (!isNumeric(algNotation.front()) && algNotation.front() != "(") {
    errorMessage = "starts with an op";
  } else if (!isNumeric(algNotation.back()) && algNotation.back() != ")") {
    errorMessage = "ends with an op";
  } else {
    for (string token : algNotation) {
      if (token == "(") {
        ++openParentheses;
      } else if (token == ")") {
        --openParentheses;
      } else if (isNumeric(token)) {
        size_t decimalCount = 0;
        for (char ch : token) {
          if (ch == '.') {
            ++decimalCount;
          }
        }
        if (token.length() == 1 && decimalCount) {
          errorMessage = "isolated decimal";
          break;
        }

        if (decimalCount > 1) {
          errorMessage = "multiple decimals";
          break;
        }
        ++previousNumeric;
        if (previousOp) {
          --previousOp;
        }
      } else {
        ++previousOp;
        if (!previousNumeric) {
          errorMessage = "consecutive ops";
          break;
        }
        --previousNumeric;
      }

      if (openParentheses < 0) {
        errorMessage = "missing open parenthesis";
        break;
      }

      if (previousNumeric > 1) {
        errorMessage = "consecutive numeric values";
        break;
      }
    }
  }

  if (!errorMessage.length() && openParentheses) {
    errorMessage = "missing closing parenthesis";
  }

  if (errorMessage.length()) {
    print(errorMessage, "Invalid: ");
    return false;
  }
  return true;
}

int main() {
  //   deque<string> algNotation;
  //   do {
  //     const string inputAsString = getString("Enter Expression: ");
  //     algNotation = tokenize(inputAsString);
  //   } while (!validAlgNotation(algNotation));

  //   const deque<string> rpnNotation = shuntingYard(algNotation);

  //   try {
  //     const double result = evalRpnNotation(rpnNotation);
  //     print(result, "Answer: ");
  //   } catch (const exception &err) {
  //     cerr << err.what() << endl;
  //   }

  // TEST
  const string inputAsString = getString("Enter Expression: ");
  const deque<string> algNotation = tokenize(inputAsString);

  for (string token : algNotation) {
    print(token);
  }
  //   const deque<string> rpnNotation = shuntingYard(algNotation);

  //   const double result = evalRpnNotation(rpnNotation);
  //   print(result, "Answer: ");
}
