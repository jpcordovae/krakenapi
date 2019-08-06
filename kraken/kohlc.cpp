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
	ofs.write(reinterpret_cast<char*>(const_cast<time_t*>(&kohlc.ktime)),sizeof(time_t));
	ofs.write(reinterpret_cast<char*>(const_cast<float*>(&kohlc.open)),sizeof(float));
	ofs.write(reinterpret_cast<char*>(const_cast<float*>(&kohlc.high)),sizeof(float));
	ofs.write(reinterpret_cast<char*>(const_cast<float*>(&kohlc.low)),sizeof(float));
	ofs.write(reinterpret_cast<char*>(const_cast<float*>(&kohlc.close)),sizeof(float));
	ofs.write(reinterpret_cast<char*>(const_cast<float*>(&kohlc.vwap)),sizeof(float));
	ofs.write(reinterpret_cast<char*>(const_cast<float*>(&kohlc.volume)),sizeof(float));
	ofs.write(reinterpret_cast<char*>(const_cast<float*>(&kohlc.count)),sizeof(float));
	return ofs;
  }

  std::ifstream& operator>>(std::ifstream &ifs, KOHLC &kohlc)
  {
	ifs.read(reinterpret_cast<char*>(&kohlc.ktime),sizeof(time_t));
	ifs.read(reinterpret_cast<char*>(&kohlc.open),sizeof(float));
	ifs.read(reinterpret_cast<char*>(&kohlc.high),sizeof(float));
	ifs.read(reinterpret_cast<char*>(&kohlc.low),sizeof(float));
	ifs.read(reinterpret_cast<char*>(&kohlc.close),sizeof(float));
	ifs.read(reinterpret_cast<char*>(&kohlc.vwap),sizeof(float));
	ifs.read(reinterpret_cast<char*>(&kohlc.volume),sizeof(float));
	ifs.read(reinterpret_cast<char*>(&kohlc.count),sizeof(float));
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
	std::for_each(kohlcs.begin(),kohlcs.end(),[&](const KOHLC &kohlc){
										ofs << kohlc << endl;
									  });
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
