#include <iostream>
#include <string>

std::string getString(const std::string &prompt) {
  std::string input = "";
  std::cout << prompt;
  std::getline(std::cin, input);

  while (input == "") {
    std::cout << "No characters entered." << std::endl;
    std::cout << prompt;
    std::getline(std::cin, input);
  }

  return input;
}
