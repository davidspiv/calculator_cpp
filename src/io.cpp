#include <termios.h>
#include <unistd.h>

#include <iostream>
#include <list>
#include <string>

#include "../include/historyCache.h"

HistoryCache historyCache;

void setNonCanonicalMode(struct termios &old_settings) {
  struct termios newSettings;
  tcgetattr(STDIN_FILENO, &old_settings);  // Get current terminal attributes
  newSettings = old_settings;

  newSettings.c_lflag &= ~(ICANON | ECHO);  // Disable canonical mode and echo
  newSettings.c_cc[VMIN] = 1;               // Read one character at a time
  newSettings.c_cc[VTIME] = 0;              // No timeout

  tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);  // Apply new settings
}

void restoreCanonicalMode(const struct termios &old_settings) {
  tcsetattr(STDIN_FILENO, TCSANOW, &old_settings);  // Restore old settings
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

  struct termios old_tio;
  std::cout << ">>  " << std::flush;
  const std::string csiCommand = "\r\033[K";
  std::string input;
  char ch;

  setNonCanonicalMode(old_tio);

  while (read(STDIN_FILENO, &ch, 1) == 1) {
    if (ch == '\n') break;  // Stop on Enter key

    if (ch == 0x7F) {  // Handle Backspace (DEL)
      if (!input.empty()) {
        input.pop_back();
        std::cout << "\b \b"
                  << std::flush;  // Move cursor back, erase char, move back
      }
    } else if (ch == '\033') {  // Possible arrow key (Escape sequence)
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
      std::cout << ch << std::flush;
    }
  }

  if ((historyCache.isBeginning() || historyCache.getCurrent() != input) &&
      !input.empty()) {
    historyCache.addEntry(input);
  }

  if (input.empty()) {
    std::cout << csiCommand;
  }

  if (input.length() != displayOneLine(input).length()) {
    std::cout << csiCommand << ">>  " << input << std::flush;
  }

  historyCache.beginning();
  restoreCanonicalMode(old_tio);
  return input;
}
