#include <deque>
#include <stack>

#include "../include/operators.h"
#include "../include/token.h"

bool isNumeric(const char symbol) { return isdigit(symbol) || symbol == '.'; }

// Determine if '-' represent negation or subtraction
bool isNegateOp(const std::deque<Token> &tokens) {
  return tokens.empty() ||
         (tokens.back().type != value && tokens.back().symbol != ")");
}

size_t safePeekTop(const std::stack<Token> &opStack) {
  if (!opStack.empty()) {
    return opRank.at(opStack.top().symbol);
  }
  return 0;
}
