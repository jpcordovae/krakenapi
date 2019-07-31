#include "kohlc.hpp"

namespace kraken {

  std::ostream& operator<<(std::ostream &os, const KOHLC &kohlc)
  {
    os << to_string(kohlc.ktime) << " ; "
       << kohlc.open << " ; "
       << kohlc.close << " ; "
       << kohlc.high << " ; "
       << kohlc.low << " ; "
       << kohlc.vwap << " ; "
       << kohlc.volume << " ; "
       << kohlc.count;
    return os;
  }

    std::ofstream& operator<<(std::ofstream &ofs,const KOHLC &kohlc)
  {
	ofs.write((char*)&kohlc.ktime,sizeof(time_t));
	ofs.write((char*)&kohlc.open,sizeof(float));
	ofs.write((char*)&kohlc.high,sizeof(float));
	ofs.write((char*)&kohlc.low,sizeof(float));
	ofs.write((char*)&kohlc.close,sizeof(float));
	ofs.write((char*)&kohlc.vwap,sizeof(float));
	ofs.write((char*)&kohlc.volume,sizeof(float));
	ofs.write((char*)&kohlc.count,sizeof(float));
	return ofs;
  }

  std::ifstream& operator>>(std::ifstream &ifs, KOHLC &kohlc)
  {
	ifs.read((char*)&kohlc.ktime,sizeof(time_t));
	ifs.read((char*)&kohlc.open,sizeof(float));
	ifs.read((char*)&kohlc.high,sizeof(float));
	ifs.read((char*)&kohlc.low,sizeof(float));
	ifs.read((char*)&kohlc.close,sizeof(float));
	ifs.read((char*)&kohlc.vwap,sizeof(float));
	ifs.read((char*)&kohlc.volume,sizeof(float));
	ifs.read((char*)&kohlc.count,sizeof(float));
	return ifs;
  }
  
  std::ostream& operator<<(std::ostream &os, const KOHLCStorage &KS)
  {
	size_t size = KS.size();
	os.write((char*)&size,sizeof(size_t));
    std::for_each(KS.begin(),KS.end(),[&os](const KOHLC &kohlc){
										os << kohlc << endl;
									  });
    return os;
  }

  std::ofstream& operator<<(std::ofstream &ofs, const KOHLCStorage &kohlcs)
  {
	size_t size = kohlcs.size();
	ofs.write(reinterpret_cast<char*>(&size),sizeof(size_t));
	return ofs;
  }

  std::ifstream& operator>>(std::ifstream &ifs, KOHLCStorage &kohlcs)
  {
	size_t size;
	KOHLC kohlc;
	ifs.read(reinterpret_cast<char*>(&size),sizeof(size_t));
	for(size_t k = 0 ; k < kohlcs.size(); k++){
	  ifs >> kohlc;
	  kohlcs.push_back(kohlc);
	}
	return ifs;
  }
  
  /*std::istream& operator>>(std::istream &ifs, KOHLCStorage &kohlcs)
  {
	size_t size;
	KOHLC kohlc;
	ifs.read((char*)&size,sizeof(size_t));
	for( size_t k=0 ; k < size ; k++ ){
	  ifs >> kohlc;
	  kohlcs.push_back(kohlc);
	}
	return ifs;
	}*/
  
};
