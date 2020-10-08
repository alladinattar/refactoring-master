// Copyright 2020 Your Name <your_email>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

class Log {
 public:
  void Write(std::string_view message) const { *out_ << message << std::endl; }

  void WriteDebug(std::string_view message) const {
    if (level_ > 0) *out_ << message << std::endl;
  }

  Log(size_t level) : level_(level) { out_ = &std::cout; }

 private:
  size_t level_ = 0;
  mutable std::ostream* out_;
};

struct Item {
  std::string id;
  std::string name;
  float score = 0;
};

constexpr size_t kMinLines = 10;

class UsedMemory {
 public:
  UsedMemory(const Log& log) : log_(&log) {}

  void OnDataLoad(const std::vector<Item>& old_items,
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

  void OnRawDataLoad(const std::vector<std::string>& old_items,
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

  size_t used() const { return used_; }

 private:
  const Log* log_;
  size_t used_ = 0;
};

class StatSender {
 public:
  StatSender(const Log& log) : log_(&log) {}
  void OnLoaded(const std::vector<Item>& new_items) {
    log_->WriteDebug("StatSender::OnDataLoad");

    AsyncSend(new_items, "/items/loaded");
  }

  void Skip(const Item& item) { AsyncSend({item}, "/items/skiped"); }

 private:
  void AsyncSend(const std::vector<Item>& items, std::string_view path) {
    log_->Write(path);
    log_->Write("send stat " + std::to_string(items.size()));

    for (const auto& item : items) {
      log_->WriteDebug("send: " + item.id);
      // ... some code
      fstr << item.id << item.name << item.score;
      fstr.flush();
    }
  }

  const Log* log_;
  std::ofstream fstr{"network", std::ios::binary};
};

class PageContainer {
 public:
  void Load(std::istream& io, float threshold) {
    std::vector<std::string> raw_data;

    while (!io.eof()) {
      std::string line;
      std::getline(io, line, '\n');
      raw_data.push_back(std::move(line));
    }

    if (raw_data.size() < kMinLines) {
      throw std::runtime_error("too small input stream");
    }

    memory_counter_->OnRawDataLoad(raw_data_, raw_data);
    raw_data_ = std::move(raw_data);

    std::vector<Item> data;
    std::set<std::string> ids;
    for (const auto& line : raw_data_) {
      std::stringstream stream(line);

      Item item;
      stream >> item.id >> item.name >> item.score;

      if (auto&& [_, inserted] = ids.insert(item.id); !inserted) {
        throw std::runtime_error("already seen");
      }

      if (item.score > threshold) {
        data.push_back(std::move(item));
      } else {
        stat_sender_.Skip(item);
      }
    }

    if (data.size() < kMinLines) {
      throw std::runtime_error("oops");
    }

    memory_counter_->OnDataLoad(data_, data);
    stat_sender_.OnLoaded(data);
    data_ = std::move(data);
  }

  const Item& ByIndex(size_t i) const { return data_[i]; }

  const Item& ById(const std::string& id) const {
    auto it = std::find_if(std::begin(data_), std::end(data_),
                           [&id](const auto& i) { return id == i.id; });
    return *it;
  }

  void Reload(float threshold) {
    std::vector<Item> data;
    std::set<std::string> ids;
    for (const auto& line : raw_data_) {
      std::stringstream stream(line);

      Item item;
      stream >> item.id >> item.name >> item.score;

      if (auto&& [_, inserted] = ids.insert(item.id); !inserted) {
        throw std::runtime_error("already seen");
      }

      if (item.score > threshold) {
        data.push_back(std::move(item));
      } else {
        stat_sender_.Skip(item);
      }
    }

    if (data.size() < kMinLines) {
      throw std::runtime_error("oops");
    }

    memory_counter_->OnDataLoad(data_, data);
    stat_sender_.OnLoaded(data);
    data_ = std::move(data);
  }

  PageContainer(const Log& log, UsedMemory* memory_counter)
      : log_(&log), memory_counter_(memory_counter), stat_sender_(*log_) {}

 private:
  const Log* log_;
  UsedMemory* memory_counter_;
  StatSender stat_sender_;
  std::vector<Item> data_;
  std::vector<std::string> raw_data_;
};

int main(int argc, char* argv[]) {
  Log the_log(0);
  UsedMemory used_memory(the_log);

  std::string filename = "data.txt";
  for (int i = 1; i < argc; ++i) {
    if (std::string(argv[i]) == "input") {
      filename = argv[i + 1];
    }
  }

  float threshold = 0.1;
  PageContainer page(the_log, &used_memory);
  std::ifstream in(filename);
  page.Load(in, threshold);

  the_log.Write(std::to_string(used_memory.used()));

  for (size_t i = 0; i < 5; ++i) {
    const auto& item = page.ByIndex(i);
    std::cout << item.name << ": " << item.score << std::endl;
    const auto& item2 = page.ById(std::to_string(i));
    std::cout << item2.name << ": " << item2.score << std::endl;
  }

  page.Reload(threshold);
  the_log.Write(std::to_string(used_memory.used()));

  return 0;
}
