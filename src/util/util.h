//
// Created by xenon on 26.04.2021.
//

#ifndef INF442_UTIL_H
#define INF442_UTIL_H

#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>

template <class T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& vec) {
  out << '[';
  bool first = true;
  for (const auto& element : vec) {
    if (!first) {
      out << ", ";
    }
    first = false;
    out << element;
  }
  return out << ']';
}

template <class Key, class Value>
std::ostream& operator<<(std::ostream& out, const std::map<Key, Value>& dict) {
  out << '{';
  bool first = true;
  for (const auto& [key, value] : dict) {
    if (!first) {
      out << ", ";
    }
    first = false;
    out << key << ": " << value;
  }
  return out << '}';
}

template <class Key, class Value>
std::ostream& operator<<(std::ostream& out,
                         const std::unordered_map<Key, Value>& dict) {
  out << '{';
  bool first = true;
  for (const auto& [key, value] : dict) {
    if (!first) {
      out << ", ";
    }
    first = false;
    out << key << ": " << value;
  }
  return out << '}';
}

#endif // INF442_UTIL_H
