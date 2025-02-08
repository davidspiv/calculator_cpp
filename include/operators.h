#ifndef OPERATORS_H
#define OPERATORS_H

#include <string>
#include <unordered_map>

const std::unordered_map<std::string, size_t> opRank = {
    {"(", 0},   {"+", 1},   {"-", 1},   {"*", 2},   {"/", 2},
    {"^", 3},   {"neg", 4}, {"sin", 5}, {"cos", 5}, {"tan", 5},
    {"csc", 5}, {"sec", 5}, {"cot", 5}};

#endif
