#include "usedMemory.hpp"
void UsedMemory::OnDataLoad(const std::vector<Item>& old_items,
                            const std::vector<Item>& new_items) {
  log_->WriteDebug("UsedMemory::OnDataLoad");
  for (const auto& item : old_items) {
    used_ -= item.id.capacity();
    used_ -= item.name.capacity();
    used_ -= sizeof(item.score);
  }

  for (const auto& item : new_items) {
    used_ += item.id.capacity();
    used_ += item.name.capacity();
    used_ += sizeof(item.score);
  }
  log_->Write("UsedMemory::OnDataLoad: new size = " + std::to_string(used_));
}

void UsedMemory::OnRawDataLoad(const std::vector<std::string>& old_items,
                               const std::vector<std::string>& new_items) {
  log_->WriteDebug("UsedMemory::OnRawDataLoads");
  for (const auto& item : old_items) {
    used_ -= item.capacity();
  }

  for (const auto& item : new_items) {
    used_ += item.capacity();
  }
  log_->Write("UsedMemory::OnDataLoad: new size = " + std::to_string(used_));
}

size_t UsedMemory::used() const { return used_; }
