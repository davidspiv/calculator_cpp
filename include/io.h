#ifndef IO_H
#define IO_H

#include <iomanip>
#include <iostream>
#include <list>
#include <string>

std::string getString(std::list<std::string> &history);

template <typename T>
void print(T output, const std::string &label = "") {
  if (label.empty()) {
    std::cout << output << std::endl;
    return;
  }
  std::cout << std::setw(18) << label << output << std::endl;
}

#endif
