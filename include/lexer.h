#ifndef LEXER_H
#define LEXER_H

#include <deque>
#include <string>

#include "../include/result.h"
#include "../include/token.h"

const TokensResult lexer(const char &symbol, std::string &valueBuff,
                         std::string &opSymbolBuff);

#endif
