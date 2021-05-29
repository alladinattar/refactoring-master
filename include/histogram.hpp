#ifndef INCLUDE_HISTOGRAM_HPP_
#define INCLUDE_HISTOGRAM_HPP_

#include <vector>
#include <string>
#include <common.hpp>
#include <log.hpp>
#include <observer.hpp>

class histogram : public observer {
 public:
  histogram() = default;

  void onDataLoad(const std::vector<item> &,
                    const std::vector<item> &) override;
  void onSkipped(const item &) override { _thrown++; }
  void onRawDataLoad(const std::vector<std::string> &,
                        const std::vector<std::string> &) override {}

  [[nodiscard]] float average() const { return _average; }
  [[nodiscard]] size_t thrown() const { return _thrown; }

 private:
  float _average = 0;
  size_t _thrown = 0;
};

#endif  // INCLUDE_HISTOGRAM_HPP_