#ifndef HISTORYCACHE_H
#define HISTORYCACHE_H

#include <iostream>
#include <list>
#include <string>

class HistoryCache {
 public:
  void addEntry(const std::string& entry);

  void moveForward();

  void moveBackward();

  void beginning();

  std::string getCurrent() const;

  bool empty();

 private:
  std::list<std::string> history;
  std::list<std::string>::iterator iter = history.end();
};

#endif
