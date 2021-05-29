// Copyright 2020 Your Name <your_email>

#ifndef INCLUDE_COMMON_HPP_
#define INCLUDE_COMMON_HPP_

#include <algorithm>
#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

constexpr size_t MIN_LINES = 10;

struct item {
 public:
  int id = -1;
  std::string name = "EMPTY";
  int score = -1;

 public:
  bool operator==(const item& rhs) const {
    if (id == rhs.id && name == rhs.name && score == rhs.score) {
      return true;
    } else {
      return false;
    }
  }

  [[nodiscard]] bool valid() const {
    if (id == -1) {
      return false;
    }
    if (name == "EMPTY") {
      return false;
    }
    if (score == -1) {
      return false;
    }
    return true;
  }

  friend std::istream& operator>>(std::istream& is, item& i) {
    char* endptr;
    std::string str{};
    is >> str;
    i.id = static_cast<int>(strtol(str.c_str(), &endptr, 10));
    if (str[0] == *endptr) {
      i.id = -1;
    }
    is >> i.name;
    str.clear();
    is >> str;
    i.score = static_cast<int>(strtol(str.c_str(), &endptr, 10));
    if (str[0] == *endptr) {
      i.score = -1;
    }
    return is;
  }
};

#endif  // INCLUDE_COMMON_HPP_