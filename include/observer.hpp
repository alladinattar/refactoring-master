#ifndef INCLUDE_I_OBSERVER_HPP_
#define INCLUDE_I_OBSERVER_HPP_

#include <common.hpp>
#include <string>
#include <vector>

class observer {
 public:
  observer() = default;
  virtual ~observer() = default;
  virtual void onDataLoad(const std::vector<item>&,
                            const std::vector<item>&) = 0;
  virtual void onRawDataLoad(const std::vector<std::string>&,
                                const std::vector<std::string>&) = 0;
  virtual void onSkipped(const item&) = 0;
};
#endif  // INCLUDE_I_OBSERVER_HPP_