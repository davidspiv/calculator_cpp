#include <deque>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "../include/io.h"

using namespace std;

enum TokenType { value, unaryOp, binaryOp };

struct Token {
  string symbol;
  TokenType type;
};

bool isNumeric(string symbol) {
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
        // symbol != " " && (!isNumeric(symbol) || symbol == ")")
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
  unordered_map<string, size_t> opRank;
  opRank["+"] = 1;
  opRank["-"] = 1;
  opRank["*"] = 2;
  opRank["/"] = 2;
  opRank["NEG"] = 3;
  opRank["("] = 0;

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
           opRank.at(token.symbol) < opRank.at(opStack.top().symbol)) {
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
  //     algNotation = lexer(inputAsString);
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
  // should still work even with invalid chars
  const deque<Token> algNotation = lexer(inputAsString);
  const deque<Token> rpnNotation = shuntingYard(algNotation);
  //   const double result = evalRpnNotation(rpnNotation);

  for (Token token : rpnNotation) {
    print(token.symbol);
  }

  //   print(result, "Answer: ");
}
