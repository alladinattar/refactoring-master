#include "statSender.hpp"

void StatSender::OnLoaded(const std::vector<Item>& new_items) {
  log_->WriteDebug("StatSender::OnDataLoad");

  AsyncSend(new_items, "/items/loaded");
}

void StatSender::Skip(const Item& item) { AsyncSend({item}, "/items/skiped"); }

void StatSender::AsyncSend(const std::vector<Item>& items, std::string_view path) {
  log_->Write(path);
  log_->Write("send stat " + std::to_string(items.size()));

  for (const auto& item : items) {
    log_->WriteDebug("send: " + item.id);
    // ... some code
    fstr << item.id << item.name << item.score;
    fstr.flush();
  }
}

