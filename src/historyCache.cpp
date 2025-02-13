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
  }
}

void HistoryCache::moveBackward() {
  if (iter != history.begin()) {
    --iter;
  }
}

void HistoryCache::beginning() { iter = history.end(); }

bool HistoryCache::empty() { return history.empty(); }

std::string HistoryCache::getCurrent() const {
  return history.empty() ? "" : *iter;
}
