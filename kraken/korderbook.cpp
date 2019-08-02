#include "korderbook.hpp"
#include <algorithm>

namespace kraken {

  void clear_korderbook(KOrderBook &_kob)
  {
	_kob.pair.clear();
	_kob.lBids.clear();
	_kob.lAsks.clear();
  }

  std::ostream& operator<<(std::ostream &oss, KOrderBook &kob)
  {
	oss << kob.pair << std::endl;
	oss << "BIDS" << std::endl;
	KOrders::iterator it;
	oss << "Bids" << std::endl;
	for(it=kob.lBids.begin();it!=kob.lBids.end();++it){
	  oss << *it << std::endl;
	}
	oss << "Asks" << std::endl;
	for(it=kob.lAsks.begin();it!=kob.lAsks.end();++it){
	  oss << *it << std::endl;
	}
  
	return oss;
  }  

  std::ofstream& operator<<(std::ofstream &ofs, const KOrderBook &kob)
  {
	return ofs << kob.pair << kob.lBids << kob.lAsks;
  }
  
  std::ifstream& operator>>(std::ifstream &ifs, KOrderBook &kob)
  {
	return ifs >> kob.pair >> kob.lBids >> kob.lAsks;
  }

  std::ofstream& operator<<(std::ofstream &ofs, const KOBStorage &kobs)
  {
	size_t size = kobs.size();
	for_each(kobs.begin(),kobs.end(),[&](KOrderBook const  &kob){
									   ofs << kob;
									 });
	return ofs;
  }
  
  std::ifstream& operator>>(std::ifstream &ifs, KOBStorage &kobs)
  {
	size_t size;
	ifs.read(reinterpret_cast<char*>(&size),sizeof(size_t));
	KOrderBook kob;
	for(size_t k=0;k<size;k++){
	  ifs >> kob;
	  kobs.push_back(kob);
	}
	return ifs;
  }
  
};
