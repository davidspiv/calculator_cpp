#ifndef SHUNTINGYARD_H
#define SHUNTINGYARD_H

#include <deque>

#include "token.h"

std::deque<Token> shuntingYard(std::deque<Token> inputQueue);

#endif
