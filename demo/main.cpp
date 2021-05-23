#include "iostream"
#include "log.hpp"
#include "usedMemory.hpp"
#include "pageContainer.hpp"

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