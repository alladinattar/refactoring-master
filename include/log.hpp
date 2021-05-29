#ifndef INCLUDE_LOG_HPP_
#define INCLUDE_LOG_HPP_

#include <common.hpp>

class log_t {
 public:
  static log_t* getInstance();
  void setLogLevel(size_t level) { level_ = level; }

 public:
  void write(std::string_view) const;
  void write_debug(std::string_view) const;

 private:
  log_t():output_{&std::cout}{}
  size_t level_ = 0;
  mutable std::ostream* output_;
};

#endif  // INCLUDE_LOG_HPP_