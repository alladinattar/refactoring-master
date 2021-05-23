#include "iostream"
class Log {
 public:
  void Write(std::string_view message) const;

  void WriteDebug(std::string_view message) const;

  explicit Log(size_t level) : level_(level) { out_ = &std::cout; }

 private:
  size_t level_ = 0;
  mutable std::ostream* out_;
};