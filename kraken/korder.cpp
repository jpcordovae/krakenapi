#include "korder.hpp"
#include <algorithm>

namespace kraken {

  std::ostream& operator<<(std::ostream &os,const KOrder &ko)
  {
	return os << "P: " << std::to_string(ko.price) << " , V: " << std::to_string(ko.volume) << " , t: " << std::to_string(ko.time);
  }

  std::ofstream& operator<<(std::ofstream &ofs, const KOrder &ko)
  {
	ofs.write(reinterpret_cast<char*>(const_cast<double*>(&ko.price)),sizeof(double));
	ofs.write(reinterpret_cast<char*>(const_cast<double*>(&ko.volume)),sizeof(double));
	ofs.write(reinterpret_cast<char*>(const_cast<time_t*>(&ko.time)),sizeof(time_t));
	ofs.write(reinterpret_cast<char*>(const_cast<KOrder::Order_t*>(&ko.otype)),sizeof(KOrder::Order_t));
	ofs << ko.misc;
	return ofs;
  }
  
  std::ifstream& operator>>(std::ifstream &ifs, KOrder &ko)
  {
	ifs.read(reinterpret_cast<char*>(&ko.price),sizeof(double));
	ifs.read(reinterpret_cast<char*>(&ko.volume),sizeof(double));
	ifs.read(reinterpret_cast<char*>(&ko.time),sizeof(time_t));
	ifs.read(reinterpret_cast<char*>(&ko.otype),sizeof(KOrder::Order_t));
	ifs >> ko.misc;
	return ifs;
  }

  std::ostream& operator<<(std::ostream &os, const KOrders &kos)
  {
	std::for_each(kos.begin(),kos.end(),[&](const KOrder &ko){
										   os << ko;
										 });
	return os;
  }
  
  std::ofstream& operator<<(std::ofstream &ofs, const KOrders &kos)
  {
	size_t size = kos.size();
	ofs.write(reinterpret_cast<char*>(&size),sizeof(size_t));
	std::for_each(kos.begin(),kos.end(),[&](const KOrder &ko){
										  ofs << ko;
										});
	return ofs;
  }
  
  std::ifstream& operator>>(std::ifstream &ifs, KOrders &kos)
  {
	size_t size;
	ifs.read(reinterpret_cast<char*>(&size),sizeof(size_t));
	KOrder ko;
	for(size_t k=0;k<size;k++) {
	  ifs >> ko;
	  kos.push_back(ko);
	}
	return ifs;
  }

  
  std::vector<double> get_prices(KOrders &kos)
  {
	std::vector<double> prices;
	for_each(kos.begin(),kos.end(),[&](KOrder &ko){
									 prices.push_back(ko.price);
								   });
	return prices;
  }
  
  std::vector<double> get_volumes(KOrders &kos)
  {
	std::vector<double> volumes;
	for_each(kos.begin(),kos.end(),[&](KOrder &ko){
									 volumes.push_back(ko.volume);
								   });
	return volumes;
  }

  std::vector<time_t> get_times(KOrders &kos)
  {
	std::vector<time_t> times;
	for_each(kos.begin(),kos.end(),[&](KOrder &ko){
									 times.push_back(ko.time);
								   });
	return times;
  }

};
