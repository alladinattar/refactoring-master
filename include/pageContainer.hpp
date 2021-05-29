#ifndef INCLUDE_PAGE_CONTAINER_HPP_
#define INCLUDE_PAGE_CONTAINER_HPP_

#include <common.hpp>
#include <histogram.hpp>
#include <log.hpp>
#include <set>
#include <statSender.hpp>
#include <string>
#include <usedMemory.hpp>
#include <utility>
#include <vector>

#include "statSender.hpp"

template <class statSender_t = statSender>
class pageContainer {
 public:
  explicit pageContainer(used_memory* counter = new usedMemory(),
                         statSender_t* sender = new statSender_t()) {
    attach(counter);
    attach(sender);
  }
  ~pageContainer() {
    for (auto observer : observers_){
      delete observer;
    }
  }

  void attach(observer* observer) { observers_.emplace_back(observer); }

 public:
  void load_raw_data(std::istream& is) {
    std::vector<std::string> rawData;
    auto log = log_t::getInstance();

    if (!is) {
      throw std::runtime_error{"unable to open file"};
    }

    log->write("opening file");
    while (!is.eof()) {
      std::string line;
      std::getline(is, line, '\n');
      rawData.push_back(std::move(line));
    }

    if (rawData.size() < MIN_LINES) {
      throw std::runtime_error("too small input stream");
    }

    onRawDataLoad(rawData);
    rawData_ = std::move(rawData);
  }
  void loadData(int threshold) {
    std::vector<item> data;
    std::set<int> ids;
    for (const auto& line : rawData_) {
      std::stringstream stream(line);

      item i;
      stream >> i;

      if (auto&& [_, inserted] = ids.insert(i.id); !inserted) {
        throw std::runtime_error("already seen");
      }

      if (i.score > threshold && i.valid()) {
        data.push_back(std::move(i));
      } else {
        onSkipped(i);
      }
    }

    if (data.size() < MIN_LINES) {
      throw std::runtime_error("oops");
    }

    onDataLoad(data);
    data_ = std::move(data);
  }

  const item& operator[](size_t i) const { return data_[i]; }
  const item& operator[](const std::string& id) const {
    auto it = std::find_if(data_.begin(), data_.end(), [&id](auto const& i) {
      return atoi(id.c_str()) == i.id;
    });
    return *it;
  }

  void onDataLoad(const std::vector<item>& new_data) {
    for (auto& observer : observers_) {
      observer->onDataLoad(data_, new_data);
    }
  }
  void onRawDataLoad(const std::vector<std::string>& new_data) {
    for (auto& observer : observers_) {
      observer->onRawDataLoad(rawData_, new_data);
    }
  }
  void onSkipped(const item& i) {
    for (auto& observer : observers_) {
      observer->onSkipped(i);
    }
  }

  [[nodiscard]] size_t data_size() const { return data_.size(); }

 private:
  std::vector<observer*> observers_;
  std::vector<item> data_;
  std::vector<std::string> rawData_;
};

#endif  // INCLUDE_PAGE_CONTAINER_HPP_