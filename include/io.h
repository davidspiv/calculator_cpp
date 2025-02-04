#ifndef IO_H
#define IO_H

#include <iomanip>
#include <iostream>
#include <string>

std::string getString(const std::string &prompt);

template <typename T>
void print(const std::string &label, T output) {
  std::cout << std::setw(18) << label << output << std::endl;
}

#endif
