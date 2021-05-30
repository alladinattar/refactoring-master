#include <log.hpp>

void log_t::write(std::string_view message) const {
  *output_ << message << '\n';
}

void log_t::write_debug(std::string_view message) const {
  if (!level_) *output_ << message << '\n';
}
log_t* log_t::getInstance() {
  static log_t instance{};
  return &instance;
}