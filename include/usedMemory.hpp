#ifndef INCLUDE_USED_MEMORY_HPP_
#define INCLUDE_USED_MEMORY_HPP_

#include <common.hpp>
#include <log.hpp>
#include <observer.hpp>
#include <vector>
#include <string>

class used_memory : public observer {
 public:
  used_memory() = default;
  ~used_memory() override = default;

  void on_data_load(const std::vector<item>&,
                    const std::vector<item>&) override;
  void on_raw_data_load(const std::vector<std::string>&,
                        const std::vector<std::string>&) override;
  void on_skipped(const item&) override {}

 public:
  [[nodiscard]] size_t used() const { return _used; }
  void clear() { _used = 0; }

 private:
  size_t _used = 0;
};
#endif  // INCLUDE_USED_MEMORY_HPP_