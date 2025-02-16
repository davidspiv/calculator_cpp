#include "../include/io.h"

#include <termios.h>
#include <unistd.h>

#include <deque>
#include <iostream>
#include <list>
#include <stdexcept>
#include <string>

#include "../include/evalRpnNotation.h"
#include "../include/lexer.h"
#include "../include/shuntingYard.h"

HistoryCache historyCache;

void setNonCanonicalMode(struct termios &initialSettings) {
  struct termios newSettings;
  tcgetattr(STDIN_FILENO, &initialSettings);  // Get current terminal attributes
  newSettings = initialSettings;

  newSettings.c_lflag &= ~(ICANON | ECHO);  // Disable canonical mode and echo
  newSettings.c_cc[VMIN] = 1;               // Read one character at a time
  newSettings.c_cc[VTIME] = 0;              // No timeout

  tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);  // Apply new settings
}

void restoreCanonicalMode(const struct termios &initialSettings) {
  tcsetattr(STDIN_FILENO, TCSANOW,
            &initialSettings);  // Restore initial settings
}

std::string displayOneLine(std::string input) {
  return input.length() < 79 ? input : input.substr(1, 76) + "...";
}

std::string getString() {
  // TEST
  if (historyCache.empty()) {
    historyCache.addEntry("1*1");
    historyCache.addEntry("2*2");
    historyCache.addEntry("3*3");
  }

  struct termios terminalSettings;
  std::cout << ">  " << std::flush;
  const std::string csiCommand = "\r\033[K";
  std::string input;
  char ch;
  bool isFirstIteration = true;

  setNonCanonicalMode(terminalSettings);

  while (read(STDIN_FILENO, &ch, 1) == 1) {
    if (ch == '\n') break;

    if (ch == 0x7F) {  // Handle backspace
      if (!input.empty()) {
        std::cout << "\b \b" << std::flush;
        input.pop_back();
      }
    } else if (ch == '\033') {  // Handle escape sequence
      char seq[2];
      if (read(STDIN_FILENO, &seq[0], 1) == 1 &&
          read(STDIN_FILENO, &seq[1], 1) == 1) {
        if (seq[0] == '[') {
          bool isSuccessful = true;
          switch (seq[1]) {
            case 'A':
              historyCache.moveBackward();
              break;

            case 'B':
              isSuccessful = historyCache.moveForward();
              break;

            default:
              continue;
          }
          input = isSuccessful ? historyCache.getCurrent() : "";
          std::cout << csiCommand << ">>  " << displayOneLine(input)
                    << std::flush;
        }
      }
    } else {  // Normal character input
      input += ch;
      std::cout << ch;
    }
    std::string result = "";
    try {
      const std::deque<Token> algNotation = lexer(input);
      const std::deque<Token> rpnNotation = shuntingYard(algNotation);
      result = std::to_string(evalRpnNotation(rpnNotation));
    } catch (const std::exception &e) {
    }

    // save cursor position, newline, erase line, grey color, writ result,
    // normal color, restore saved cursor, up one line
    std::cout << "\0337" << "\n\033[0K"                 // Newline + Clear line
              << "\033[38;5;8m" << result << "\033[0m"  // Greyed-out result
              << "\0338";                               // Restore cursor

    if (isFirstIteration) {
      std::cout << "\033[1A";  // Move cursor up one line
    }

    std::cout << std::flush;

    isFirstIteration = false;
  }

  restoreCanonicalMode(terminalSettings);

  if (input.empty()) {
    std::cout << csiCommand;
    return "";
  }

  if (historyCache.isBeginning() || historyCache.getCurrent() != input) {
    historyCache.addEntry(input);
  }

  if (input.length() != displayOneLine(input).length()) {
    std::cout << csiCommand << ">  " << input << std::flush;
  }

  historyCache.beginning();
  return input;
}
