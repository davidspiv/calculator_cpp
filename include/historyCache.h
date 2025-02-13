#ifndef HISTORYCACHE_H
#define HISTORYCACHE_H

#include <iostream>
#include <list>
#include <string>

class HistoryCache {
 public:
  void addEntry(const std::string& entry);

  bool moveForward();

  void moveBackward();

  void beginning();

  bool isBeginning();

  std::string getCurrent() const;

  bool empty();

 private:
  std::list<std::string> history;
  std::list<std::string>::iterator iter = history.end();
};

#endif
