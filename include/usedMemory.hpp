#ifndef INCLUDE_USED_MEMORY_HPP_
#define INCLUDE_USED_MEMORY_HPP_

#include <common.hpp>
#include <log.hpp>
#include <observer.hpp>
#include <vector>
#include <string>

class usedMemory : public observer {
 public:
  usedMemory() = default;
  ~usedMemory() override = default;

  void onDataLoad(const std::vector<item>&,
                    const std::vector<item>&) override;
  void onRawDataLoad(const std::vector<std::string>&,
                        const std::vector<std::string>&) override;
  void onSkipped(const item&) override {}

 public:
  [[nodiscard]] size_t used() const { return used_; }
  void clear() { used_ = 0; }

 private:
  size_t used_ = 0;
};
#endif  // INCLUDE_USED_MEMORY_HPP_