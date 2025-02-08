#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum TokenType { value, unaryOp, binaryOp, parenthesis };

struct Token {
  std::string symbol;
  TokenType type;
};

#endif
