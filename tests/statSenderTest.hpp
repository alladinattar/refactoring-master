#pragma once

#include <gmock/gmock.h>
#include <statSender.hpp>

class mock_stat_sender : public statSender {
  //  virtual void async_send(const std::vector<item>&,
  //                          std::string_view);
 public:
  MOCK_METHOD2(async_send, void(const std::vector<item>&, std::string_view));
};