#ifndef KUTILS_H__
#define KUTILS_H__

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <mutex>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <set>

namespace kraken{

  namespace fs = std::filesystem;

  class SafePrint: public std::ostringstream
  {
  public:
    SafePrint() = default;
    ~SafePrint();

  private:
    static std::mutex _mutexSafePrint;
  };

  std::ofstream& operator<<(std::ofstream &ofs, const std::string &str);
  std::ifstream& operator>>(std::ifstream &ifs, std::string &str);

  struct filename_string{
    std::string operator()(const fs::directory_entry &entry) {
      return entry.path().filename();
    }
  };

  void get_files_from_directory(const std::string &pname, std::vector<std::string> &v);

}

#endif
