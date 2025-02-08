#ifndef HELPERS_H
#define HELPERS_H

#include <deque>
#include <stack>

#include "../include/token.h"

bool isNumeric(const char symbol);

// Determine if '-' represent negation or subtraction
bool isNegateOp(const std::deque<Token> &tokens);

size_t safePeekTop(const std::stack<Token> &opStack);

#endif
