#include <iostream>
#include <list>
#include <string>

class HistoryCache {
 public:
  void addEntry(const std::string& entry);

  void moveForward();

  void moveBackward();

  void showCurrent() const;

 private:
  std::list<std::string> history;
  std::list<std::string>::iterator iter = history.end();
};
