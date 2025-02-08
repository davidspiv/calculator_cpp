#ifndef LEXER_H
#define LEXER_H

#include <deque>
#include <string>

#include "../include/token.h"
std::deque<Token> lexer(const std::string &inputAsString);

#endif
