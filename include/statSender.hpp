#include <fstream>
#include "log.hpp"
#include "structs.hpp"
#include "vector"
class StatSender {
 public:
  StatSender(const Log& log) : log_(&log) {}
  void OnLoaded(const std::vector<Item>& new_items);

  void Skip(const Item& item);

 private:
  void AsyncSend(const std::vector<Item>& items, std::string_view path);

  const Log* log_;
  std::ofstream fstr{"network", std::ios::binary};
};