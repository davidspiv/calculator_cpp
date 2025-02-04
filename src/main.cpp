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

double evalRpnNotation(const deque<Token> &rpnNotation) {
  stack<double> result;

  for (const Token &token : rpnNotation) {
    if (token.type == value) {
      result.push(stod(token.symbol));
    } else {
      const double operandB = result.top();
      result.pop();

      if (token.symbol == "NEG") {
        result.push(operandB * -1.0);
        continue;
      }

      const double operandA = result.top();
      result.pop();

      if (token.symbol == "+") {
        result.push(operandA + operandB);
      } else if (token.symbol == "-") {
        result.push(operandA - operandB);
      } else if (token.symbol == "*") {
        result.push(operandA * operandB);
      } else if (token.symbol == "/") {
        if (!operandB && operandA) {
          throw invalid_argument("ERROR: unable to divide by zero");
        }
        result.push(operandA / operandB);
      } else {
        throw invalid_argument("ERROR: unrecognized non-numeric");
      }
    }
  }
  return result.top();
}

bool validAlgNotation(const deque<Token> &algNotation) {
  int openParentheses = 0;
  size_t previousNumeric = 0;
  size_t previousOp = 0;
  string errorMessage;

  if (algNotation.empty()) {
    errorMessage = "empty input";
  } else if (algNotation.front().type == binaryOp &&
             algNotation.front().symbol != "(") {
    errorMessage = "starts with an op";
  } else if (algNotation.back().type == binaryOp &&
             algNotation.back().symbol != ")") {
    errorMessage = "ends with an op";
  } else {
    for (const Token &token : algNotation) {
      print(token.symbol);
      if (token.symbol == "(") {
        ++openParentheses;
      } else if (token.symbol == ")") {
        --openParentheses;
      } else if (token.type != value) {
        size_t decimalCount = 0;
        for (char ch : token.symbol) {
          if (ch == '.') {
            ++decimalCount;
          }
        }
        if (token.symbol.length() == 1 && decimalCount) {
          errorMessage = "isolated decimal";
          break;
        }

        if (decimalCount > 1) {
          errorMessage = "multiple decimals";
          break;
        }

        if (previousOp) {
          ++previousNumeric;
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
  const double result = evalRpnNotation(rpnNotation);

  // for (const Token &token : algNotation) {
  //   print(token.symbol);
  // }

  print(result, "Answer: ");
}
