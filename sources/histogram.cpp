#include <histogram.hpp>

void histogram::onDataLoad(const std::vector<item> &,
                             const std::vector<item> & newItems) {
  for (const auto& newItem : newItems) {
    _average += static_cast<float>(newItem.score);
  }
  _average /= static_cast<float>(newItems.size());

  log_t::getInstance()->write("Average: " + std::to_string(_average) +
                               " Thrown: " + std::to_string(_thrown));
  _thrown = 0;
}