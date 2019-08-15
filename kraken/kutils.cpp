#include "kutils.hpp"
#include <string.h>

namespace kraken {

  SafePrint::~SafePrint()
  {
    std::lock_guard<std::mutex> guard(_mutexSafePrint);
    std::cout << this->str();
  }

  std::mutex SafePrint::_mutexSafePrint{};

  std::ofstream& operator<<(std::ofstream &ofs, const std::string &str)
  {
    size_t size = str.size();
    ofs.write((char*)&size,sizeof(size_t));
    ofs.write((char*)str.c_str(),size);
    return ofs;
  }

  std::ifstream& operator>>(std::ifstream &ifs, std::string &str)
  {
    size_t size;
    ifs.read((char *)&size,sizeof(size_t));
    char *buffer = (char*) new char[size+1];
    memset(buffer,0x00,size+1);//just in case
    ifs.read(buffer,size);
    delete[] buffer;
    return ifs;
  }

  void get_files_from_directory(const std::string &pname, std::vector<std::string> &v)
  {

    fs::path p(pname);
    fs::directory_iterator di(p);

    std::transform(fs::begin(di),fs::end(di),std::back_inserter(v),[&](const fs::directory_entry &de){
        return de.path().filename();
      });
    //std::copy_if(di.begin(),di.end(),std::back_inserter(v),[&](const fs::directory_entry &entry){
    //    return entry.is_regular_file();
    //  });
  }



}
