#ifndef RESULT_H
#define RESULT_H

#include <deque>
#include <optional>
#include <string>

#include "token.h"

struct TokensResult {
  std::deque<Token> tokens;
  std::string errMessage;
};

struct NumericResult {
  double numeric;
  std::string errMessage;
};

#endif
