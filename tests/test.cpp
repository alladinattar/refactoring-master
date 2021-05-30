// Copyright 2020 Your Name <your_email>

#include <gmock/gmock.h>

#include <pageContainer.hpp>
#include "statSenderTest.hpp"
#include <usedMemory.hpp>

TEST(MemoryUsage, MemUsingTest) {
  std::vector<std::string> oldRawData{ "first" },
      new_raw_data {
      "first", "second", "third", "fourth"
  };
  usedMemory memory;
  memory.onRawDataLoad(oldRawData, new_raw_data);
  EXPECT_EQ(memory.used(), 45);

  memory.clear();

  std::vector<item> oldData{},
      new_data{
      {1, "rand", 10},
      {2, "rand", 20},
      {3, "rand", 30}
  };
  memory.onDataLoad(oldData, new_data);
  EXPECT_EQ(memory.used(), 69);
}

void prepareStringstream(std::stringstream& ss, size_t lines = 20){
  for (size_t i = 0; i < lines; ++i) {
    ss << (i + 1) << " cor " << (i < 6 ? 25 : 50) << '\n';
  }
  ss << 21 << " inc "
     << "ls" << '\n';
  ss << 22 << " inc "
     << "la" << '\n';
}

TEST(PageContainer_Test, Data_Test) {
  srand(time(nullptr));
  std::stringstream ss{};
  prepareStringstream(ss);

  pageContainer page;
  page.loadRawData(ss);
  page.loadData(0);
  EXPECT_EQ(page.dataSize(), 20);
}

TEST(PageContainer_Test, AsyncSendTest) {
  using ::testing::_;
  using ::testing::AtLeast;

  srand(time(nullptr));
  std::stringstream ss{};
  prepareStringstream(ss);

  usedMemory* used = new usedMemory();
  mock_stat_sender* sender = new mock_stat_sender();
  pageContainer<mock_stat_sender> page(used, sender);

  page.loadRawData(ss);
  EXPECT_CALL(*sender,
              async_send(_, std::string_view{"/items/loaded"}))
      .Times(2);
  EXPECT_CALL(*sender,
              async_send(_, std::string_view{"/items/skipped"}))
      .Times(12);
  page.loadData(0);
  page.loadData(30);
}

TEST(PageContainer_Test, AlreadySeen){
  std::stringstream ss{};
  prepareStringstream(ss);
  ss << 20 << " cop " << 20 << '\n';

  pageContainer page{};
  page.loadRawData(ss);
  EXPECT_THROW(page.loadData(0), std::runtime_error);
}

TEST(PageContainer_Test, EmptyFile){
  std::ifstream file_{"empty.txt"};
  std::ofstream empty{"empty.txt"};
  pageContainer page{};
  EXPECT_THROW(page.loadRawData(file_), std::runtime_error);
  empty.close();
}
TEST(PageContainer_Test, CorruptedFile){
  std::ifstream file_;
  file_.close();
  pageContainer page{};
  EXPECT_THROW(page.loadRawData(file_), std::runtime_error);
}

TEST(PageContainer_Test, TooSmallInputStream){
  std::stringstream ss{};
  prepareStringstream(ss, 5);

  pageContainer page{};
  EXPECT_THROW(page.loadRawData(ss), std::runtime_error);
}
TEST(PageContainer_Test, TooSmallInputStream_LoadData){
  std::stringstream ss{};
  prepareStringstream(ss);

  pageContainer page{};
  page.loadRawData(ss);
  EXPECT_THROW(page.loadData(1000),std::runtime_error);
}
