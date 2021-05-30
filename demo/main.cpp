#include <pageContainer.hpp>

void parse_command_line(int argc, char** argv, int& threshold, int& log_level, std::string& filename) {
  namespace po = boost::program_options;
  po::options_description desc("Allowed options");
  desc.add_options()("help,h", "Shows help message")(
      "input,i", po::value<std::string>(),
      "Sets input filename")("threshold,t", po::value<int>(),
                             "Sets threshold. Default = 0")("log_level,l",
                                                            po::value<int>(),
                                                            "Sets logging level <Debug|Info>. Default = Debug");

  po::variables_map vm{};
  po::parsed_options parsed = po::command_line_parser(argc, argv)
      .options(desc)
      .allow_unregistered()
      .run();
  po::store(parsed, vm);
  po::notify(vm);

  if (vm.count("help,h")){
    std::cout << "Usage:\n    ./demo <options>\n" << desc << "\n\n";
    exit(0);
  }

  if (vm.count("input")){
    filename = vm.at("input").as<std::string>();
  } else {
    throw std::runtime_error { "Not set input filename"};
  }

  if (vm.count("threshold")){
    threshold = vm.at("threshold").as<int>();
  }
  if(vm.count("log_level")){
    log_level = vm.at("log_level").as<int>();
  }

  std::cout << "Settings:\nFilename:\t" << filename
            << "\nThreshold:\t" << threshold
            << "\nLog Level:\t" << (log_level ? "Info" : "Debug")
            << "\n\n";
}

int main(int argc, char** argv) {
  int threshold = 0;
  int log_level = 0;
  std::string filename{};
  parse_command_line(argc, argv, threshold, log_level, filename);
  auto log = log_t::getInstance();
  log->setLogLevel(log_level);

  usedMemory used_memory{};
  histogram histogram{};
  pageContainer page(&used_memory);
  page.attach(&histogram);

  std::ifstream file(filename);

  page.loadRawData(file);
  page.loadData(threshold);

  log->write(std::to_string(used_memory.used()));

  for (size_t i = 0; i < 5; ++i){
    auto& item = page[i];
    std::cout << item.name << ": " << item.score << '\n';
    auto& item_2 = page[std::to_string(i)];
    std::cout << item_2.name << ": " << item_2.score << '\n';
  }

  page.loadData(threshold + 5);
  log->write(std::to_string(used_memory.used()));
}