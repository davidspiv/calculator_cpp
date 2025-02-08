#include <deque>
#include <iostream>
#include <string>

#include "../include/_math.h"
#include "../include/token.h"

bool isNumeric(const char symbol) { return isdigit(symbol) || symbol == '.'; }

// Determine if '-' represent negation or subtraction
bool isNegateOp(const std::deque<Token> &tokens) {
  return tokens.empty() ||
         (tokens.back().type != value && tokens.back().symbol != ")");
}

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
