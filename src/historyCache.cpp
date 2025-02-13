#include "../include/historyCache.h"

#include <iostream>
#include <list>
#include <string>

void HistoryCache::addEntry(const std::string& entry) {
  history.push_back(entry);
  iter = std::prev(history.end());  // Move iterator to the newest entry
}

void HistoryCache::moveForward() {
  if (iter != std::prev(history.end())) {
    ++iter;
  } else {
    std::cout << "End of history.\n";
  }
}

void HistoryCache::moveBackward() {
  if (iter != history.begin()) {
    --iter;
  } else {
    std::cout << "Beginning of history.\n";
  }
}

void HistoryCache::showCurrent() const {
  if (!history.empty()) {
    std::cout << "Current: " << *iter << '\n';
  } else {
    std::cout << "History is empty.\n";
  }
}
