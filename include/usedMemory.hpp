#include "log.hpp"
#include "vector"
#include "structs.hpp"
class UsedMemory {
 public:
  explicit UsedMemory(const Log& log) : log_(&log){}

  void OnDataLoad(const std::vector<Item>& old_items,
                  const std::vector<Item>& new_items);
  void OnRawDataLoad(const std::vector<std::string>& old_items,
                     const std::vector<std::string>& new_items);

  size_t used() const ;

 private:
  const Log* log_;
  size_t used_ = 0;
};