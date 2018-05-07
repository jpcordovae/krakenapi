#include "korder.hpp"
#include <algorithm>

namespace kraken {

std::ostream& operator<<(std::ostream &os,const KOrder &ko)
{
  return os << "P: " << std::to_string(ko.price) << " , V: " << std::to_string(ko.volume) << " , t: " << std::to_string(ko.time);
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
