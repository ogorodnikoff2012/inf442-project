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
  const char* prefix = "[";
  for (const auto& element : vec) {
    out << prefix << element;
    prefix = ", ";
  }
  return out << ']';
}

template <class Key, class Value>
std::ostream& operator<<(std::ostream& out, const std::map<Key, Value>& dict) {
  const char* prefix = "{";
  for (const auto& [key, value] : dict) {
    out << prefix << key << ": " << value;
    prefix = ", ";
  }
  return out << '}';
}

template <class Key, class Value>
std::ostream& operator<<(std::ostream& out,
                         const std::unordered_map<Key, Value>& dict) {
  const char* prefix = "{";
  for (const auto& [key, value] : dict) {
    out << prefix << key << ": " << value;
    prefix = ", ";
  }
  return out << '}';
}

#endif // INF442_UTIL_H
