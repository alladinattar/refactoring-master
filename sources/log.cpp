#include "log.hpp"
#include "iostream"

void Log::Write(std::string_view message) const {
  *out_ << message << std::endl;
}

void Log::WriteDebug(std::string_view message) const {
  if (level_ > 0) *out_ << message << std::endl;
}
