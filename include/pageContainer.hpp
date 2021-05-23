#include "iostream"
#include "usedMemory.hpp"
#include "log.hpp"
#include "statSender.hpp"
#include "vector"
#include "structs.hpp"
class PageContainer {
 public:
  void Load(std::istream& io, float threshold);

  const Item& ByIndex(size_t i) const;

  const Item& ById(const std::string& id) const;

  void Reload(float threshold);

  PageContainer(const Log& log, UsedMemory* memory_counter)
      : log_(&log), memory_counter_(memory_counter), stat_sender_(*log_) {}

 private:
  const Log* log_;
  UsedMemory* memory_counter_;
  StatSender stat_sender_;
  std::vector<Item> data_;
  std::vector<std::string> raw_data_;
};