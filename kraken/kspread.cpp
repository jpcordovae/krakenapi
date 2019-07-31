#include "kspread.hpp"

namespace kraken {

  std::ostream& operator<<(std::ostream &os, const KSpread &ks)
  {
    os << std::to_string(ks.time)
       << " , " << ks.bid
       << " , " << ks.ask;
    return os;
  }

  std::ofstream& operator<<(std::ofstream &ofs, const KSpread &ks)
  {
    ofs.write((char*)&ks.time,sizeof(time_t));
    ofs.write((char*)&ks.bid,sizeof(float));
    ofs.write((char*)&ks.ask,sizeof(float));
    return ofs;
  }

  std::ifstream& operator<<(std::ifstream &ifs, KSpread &ks)
  {
    ifs.read((char*)&ks.time,sizeof(std::time_t));
    ifs.read((char*)&ks.bid,sizeof(float));
    ifs.read((char*)&ks.ask,sizeof(float));
    return ifs;
  }

  std::ofstream& operator<<(std::ofstream &ofs, const KSpreadStorage &kss)
  {
    size_t size = kss.size();
    ofs.write((char*)&size,sizeof(size_t));
    for(size_t k=0 ; k < kss.size() ; k++) {
      ofs << kss[k];
    }
    return ofs;
  }

  std::ifstream& operator>>(std::ifstream &ifs, KSpreadStorage &kss)
  {
    size_t size;
    ifs.read((char*)&size,sizeof(size_t));
    KSpread ks;
    for(int k=0; k<size; k++){
      ifs >> ks;
      kss.push_back(ks);
    }
	return ifs;
  }

};//namespace kraken
