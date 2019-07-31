#include "kutils.hpp"
#include <string.h>

namespace kraken {

  SafePrint::~SafePrint()
  {
    std::lock_guard<std::mutex> guard(_mutexSafePrint);
    std::cout << this->str();
  }

  std::ofstream& operator<<(std::ofstream &ofs, const std::string &str)
  {
    size_t size = str.size();
    ofs.write((char*)&size,sizeof(size_t));
    ofs.write((char*)str.c_str(),size);
    return ofs;
  }

  std::ifstream& operator>>(std::ifstream &ifs, const std::string &str)
  {
    size_t size;
    ifs.read((char *)&size,sizeof(size_t));
    char *buffer = (char*) new char[size+1];
    memset(buffer,0x00,size+1);//just in case
    ifs.read(buffer,size);
    delete[] buffer;
    return ifs;
  }

}
