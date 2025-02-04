#ifndef IO_H
#define IO_H

#include <iomanip>
#include <iostream>
#include <string>

std::string getString(const std::string &prompt);

template <typename T>
void print(T output, const std::string &label = "") {
  if (label.empty()) {
    std::cout << output << std::endl;
    return;
  }
  std::cout << std::setw(18) << label << output << std::endl;
}

#endif
