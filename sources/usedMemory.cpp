#include <usedMemory.hpp>

void used_memory::on_data_load(const std::vector<item> & old_items,
                               const std::vector<item> & new_items) {
  auto log = log_t::get_instance();
  log->write_debug("used_memory::on_data_load");
  log->write_debug("used_memory::on_data_load: old size = " +
                   std::to_string(_used));

  for (auto const& item : new_items) {
    _used += sizeof(item.id);
    _used += item.name.capacity();
    _used += sizeof(item.score);
  }

  for (auto const& item : old_items) {
    _used -= sizeof(item.id);
    _used -= item.name.capacity();
    _used -= sizeof(item.score);
  }
  log->write("used_memory::on_data_load: new size = " + std::to_string(_used));
  log->write_debug("used_memory::on_data_load");
}

void used_memory::on_raw_data_load(const std::vector<std::string>& old_items,
                                   const std::vector<std::string>& new_items) {
  auto log = log_t::get_instance();
  log->write_debug("used_memory::on_raw_data_load");
  log->write_debug("used_memory::on_raw_data_load: old size = " +
                   std::to_string(_used));

  for (auto const& item : old_items) {
    _used -= item.capacity();
  }
  for (auto const& item : new_items) {
    _used += item.capacity();
  }


  log->write_debug("used_memory::on_raw_data_load: new size = " +
                   std::to_string(_used));
  log->write_debug("used_memory::on_raw_data_load");
}