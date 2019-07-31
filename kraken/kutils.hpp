#ifndef KUTILS_H__
#define KUTILS_H__

#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <mutex>

namespace kraken{

  class SafePrint: public std::ostringstream
  {
  public:
    SafePrint() = default;
    ~SafePrint();

  private:
    static std::mutex _mutexSafePrint;
  };

  std::ofstream& operator<<(std::ofstream &ofs, const std::string &str);
  std::ifstream& operator>>(std::ifstream &ifs, const std::string &str);

}

#endif
