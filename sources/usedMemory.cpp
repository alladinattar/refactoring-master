#include <usedMemory.hpp>

void usedMemory::onDataLoad(const std::vector<item> & old_items,
                               const std::vector<item> & new_items) {
  auto log = log_t::getInstance();
  log->write_debug("used_memory::on_data_load");
  log->write_debug("used_memory::on_data_load: old size = " +
                   std::to_string(used_));

  for (auto const& item : new_items) {
    used_ += sizeof(item.id);
    used_ += item.name.capacity();
    used_ += sizeof(item.score);
  }

  for (auto const& item : old_items) {
    used_ -= sizeof(item.id);
    used_ -= item.name.capacity();
    used_ -= sizeof(item.score);
  }
  log->write("used_memory::on_data_load: new size = " + std::to_string(used_));
  log->write_debug("used_memory::on_data_load");
}

void usedMemory::onRawDataLoad(const std::vector<std::string>& old_items,
                                   const std::vector<std::string>& new_items) {
  auto log = log_t::getInstance();
  log->write_debug("used_memory::on_raw_data_load");
  log->write_debug("used_memory::on_raw_data_load: old size = " +
                   std::to_string(used_));

  for (auto const& item : old_items) {
    used_ -= item.capacity();
  }
  for (auto const& item : new_items) {
    used_ += item.capacity();
  }


  log->write_debug("used_memory::on_raw_data_load: new size = " +
                   std::to_string(used_));
  log->write_debug("used_memory::on_raw_data_load");
}